#pragma once

#include "./VEFM_Common.h"

#include <set>
using namespace std;

#pragma once

#include "./VEFM_Common.h"

#include <set>
using namespace std;

namespace VEFM
{
	template<typename T>
	class OctreeNode
	{
	public:
		enum Index { LFU, RFU, LBU, RBU, LFL, RFL, LBL, RBL, INDEX_SIZE };

		OctreeNode(const OctreeNode<T>* pParent, int level, const V3& min, const V3& max)
			: m_pParent(pParent), m_level(level), m_min(min), m_center((min + max) * 0.5f), m_max(max)
		{
			for (int i = 0; i < INDEX_SIZE; i++)
			{
				m_children[i] = nullptr;
			}
		}

		~OctreeNode()
		{
			for (int i = 0; i < INDEX_SIZE; i++)
			{
				if (m_children[i] != nullptr)
				{
					delete m_children[i];
					m_children[i] = nullptr;
				}
			}
		}

		OctreeNode<T>* GetChild(Index index)
		{
			if (m_children[index] != nullptr)
			{
				return m_children[index];
			}
			else
			{
				return nullptr;
			}
		}

		OctreeNode<T>* GetOrCreateChild(Index index)
		{
			auto pChild = GetChild(index);
			if (pChild != nullptr)
			{
				return pChild;
			}
			else
			{
				m_children[index] = new OctreeNode<T>(this);
				return m_children[index];
			}
		}

		void Insert(T* pT, const V3& position, int maxLevel)
		{
			if (m_level != maxLevel)
			{
				auto index = DetermineIndex(position);
				if (m_children[index] == nullptr)
				{
					V3 min;
					V3 max;
					GetMinMaxByIndex(index, min, max);
					m_children[index] = new OctreeNode<T>(this, m_level + 1, min, max);
				}
				m_children[index]->Insert(pT, position, maxLevel);
			}
			else
			{
				m_elements.insert(pT);
			}
		}

		Index DetermineIndex(const V3& position)
		{
			if (position.x <= m_center.x)
				if (position.y <= m_center.y)
					if (position.z <= m_center.z) return Index::LFL;
					else return Index::LFU;
				else
					if (position.z <= m_center.z) return Index::LBL;
					else return Index::LBU;
			else
				if (position.y <= m_center.y)
					if (position.z <= m_center.z) return Index::RFL;
					else return Index::RFU;
				else
					if (position.z <= m_center.z) return Index::RBL;
					else return Index::RBU;
		}

		void GetMinMaxByIndex(Index index, V3& min, V3& max)
		{
			switch (index)
			{
			case Index::LFL:
				min = m_min;
				max = m_center;
				break;
			case Index::LFU:
				min.x = m_min.x;
				min.y = m_min.y;
				min.z = m_center.z;
				max.x = m_center.x;
				max.y = m_center.y;
				max.z = m_max.z;
				break;
			case Index::LBL:
				min.x = m_min.x;
				min.y = m_center.y;
				min.z = m_min.z;
				max.x = m_center.x;
				max.y = m_max.y;
				max.z = m_center.z;
				break;
			case Index::LBU:
				min.x = m_min.x;
				min.y = m_center.y;
				min.z = m_center.z;
				max.x = m_center.x;
				max.y = m_max.y;
				max.z = m_max.z;
				break;
			case Index::RFL:
				min.x = m_center.x;
				min.y = m_min.y;
				min.z = m_min.z;
				max.x = m_max.x;
				max.y = m_center.y;
				max.z = m_center.z;
				break;
			case Index::RFU:
				min.x = m_center.x;
				min.y = m_min.y;
				min.z = m_center.z;
				max.x = m_max.x;
				max.y = m_center.y;
				max.z = m_max.z;
				break;
			case Index::RBL:
				min.x = m_center.x;
				min.y = m_center.y;
				min.z = m_min.z;
				max.x = m_max.x;
				max.y = m_max.y;
				max.z = m_center.z;
				break;
			case Index::RBU:
				min = m_center;
				max = m_max;
				break;
			default:
				break;
			}
		}

		inline int GetLevel() const { return m_level; }
		inline const V3& GetMin() const { return m_min; }
		inline const V3& GetCenter() const { return m_center; }
		inline const V3& GetMax() const { return m_max; }

		inline const OctreeNode<T>** GetChildren() const { return (const OctreeNode<T>**)m_children; }

		inline set<T*>& GetElements() { return m_elements; }

		bool ChildExists() const
		{
			for (int i = 0; i < INDEX_SIZE; i++)
			{
				if (m_children[i] != nullptr)
					return true;
			}
			return false;
		}

		T* Find(const V3& position, int maxLevel)
		{
			if (m_level != maxLevel)
			{
				auto index = DetermineIndex(position);
				if (m_children[index] != nullptr)
				{
					return m_children[index]->Find(position, maxLevel);
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				for (auto& pV : m_elements)
				{
					if ((pV->P() - position).length() == 0)
					{
						return pV;
					}
				}

				return nullptr;
			}
		}

		T* FindNearest(const V3& position, int maxLevel)
		{
			if (m_level != maxLevel - 1)
			{
				auto index = DetermineIndex(position);
				if (m_children[index] != nullptr)
				{
					return m_children[index]->Find(position, maxLevel);
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				set<T*> elements;
				for (int i = 0; i < INDEX_SIZE; i++)
				{
					if (m_children[i] != nullptr)
					{
						auto& childElement = m_children[i]->GetElements();
						for (auto& t : childElement)
						{
							elements.insert(t);
						}
					}
				}

				float distance = -FLT_MAX;
				T* nearest = nullptr;
				for (auto& pV : elements)
				{
					if ((pV->P() - position).length() < distance)
					{
						nearest = pV;
					}
				}

				return nearest;
			}
		}

		void Erase(T* pT, const V3& position, int maxLevel)
		{
			if (m_level == maxLevel)
			{
				if (m_elements.count(pT) != 0)
				{
					m_elements.erase(pT);
				}
			}
			else
			{
				for (int i = 0; i < INDEX_SIZE; i++)
				{
					if (m_children[i] != nullptr)
					{
						m_children[i]->Erase(pT, position, maxLevel);
					}
				}
			}
		}

	private:
		int m_level = 0;
		set<T*> m_elements;
		const OctreeNode<T>* m_pParent = nullptr;
		union
		{
			OctreeNode<T>* m_children[8];
			struct
			{
				OctreeNode<T>* m_pLFU;
				OctreeNode<T>* m_pRFU;
				OctreeNode<T>* m_pLBU;
				OctreeNode<T>* m_pRBU;
				OctreeNode<T>* m_pLFL;
				OctreeNode<T>* m_pRFL;
				OctreeNode<T>* m_pLBL;
				OctreeNode<T>* m_pRBL;
			};
		};

		V3 m_min = V3::Max();
		V3 m_center = V3();
		V3 m_max = -V3::Max();
	};

	template<typename T>
	class Octree
	{
	public:
		Octree(const V3& min, const V3& max, int maxLevel)
			: m_maxLevel(maxLevel)
		{
			m_pRootNode = new OctreeNode<T>(nullptr, 0, min, max);
		}

		~Octree()
		{
			if (m_pRootNode != nullptr)
			{
				delete m_pRootNode;
				m_pRootNode = nullptr;
			}
		}

		void Insert(T* pT, const V3& position)
		{
			m_pRootNode->Insert(pT, position, m_maxLevel);
		}

		T* Find(const V3& position)
		{
			return m_pRootNode->Find(position, m_maxLevel);
		}

		T* FindNearest(const V3& position)
		{
			/*	T* pResult = nullptr;

			int increase = 0;
			while (pResult == nullptr && increase < m_maxLevel + 1)
			{
			pResult = m_pRootNode->FindNearest(position, m_maxLevel - increase);
			increase++;
			}

			return pResult;*/
			//// TODO
			return nullptr;
		}

		void Erase(T* pT, const V3& position)
		{
			return m_pRootNode->Erase(pT, position, m_maxLevel);
		}

		inline OctreeNode<T>* GetRootNode() const { return m_pRootNode; }

		inline const V3& GetMin() const { if (m_pRootNode) return m_pRootNode->GetMin(); }
		inline const V3& GetCenter() const { if (m_pRootNode) return m_pRootNode->GetCenter(); }
		inline const V3& GetMax() const { if (m_pRootNode) return m_pRootNode->GetMax(); }

	private:
		OctreeNode<T>* m_pRootNode = nullptr;
		int m_maxLevel = 5;
	};

}
