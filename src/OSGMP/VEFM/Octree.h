#pragma once

#include "./VEFM_Common.h"

#define EPSILON 0.000001f

namespace VEFM
{
	enum OctreeNodeIndex { LFU, RFU, LBU, RBU, LFL, RFL, LBL, RBL, INDEX_SIZE };

	template<typename T, typename U>
	class OctreeNode
	{
	public:
		OctreeNode(const OctreeNode<T, U>* pParent, int level, const U& min, const U& max);
		~OctreeNode();

		OctreeNode<T, U>* GetChild(OctreeNodeIndex index);
		OctreeNode<T, U>* GetOrCreateChild(OctreeNodeIndex index);

		void GetAllElements(set<T*>& elements);

		void Insert(T* pT, const U& position, int maxLevel);

		OctreeNodeIndex DetermineIndex(const U& position);

		void GetMinMaxByIndex(OctreeNodeIndex index, U& min, U& max);

		inline int GetLevel() const { return m_level; }
		inline const U& GetMin() const { return m_min; }
		inline const U& GetCenter() const { return m_center; }
		inline const U& GetMax() const { return m_max; }

		inline const OctreeNode<T, U>** GetChildren() const { return (const OctreeNode<T, U>**)m_children; }

		inline set<T*>& GetElements() { return m_elements; }

		bool ChildExists() const;

		T* Find(const U& position, int maxLevel);
		void Erase(T* pT, const U& position, int maxLevel);

	private:
		int m_level = 0;
		set<T*> m_elements;
		const OctreeNode<T, U>* m_pParent = nullptr;
		union
		{
			OctreeNode<T, U>* m_children[8];
			struct
			{
				OctreeNode<T, U>* m_pLFU;
				OctreeNode<T, U>* m_pRFU;
				OctreeNode<T, U>* m_pLBU;
				OctreeNode<T, U>* m_pRBU;
				OctreeNode<T, U>* m_pLFL;
				OctreeNode<T, U>* m_pRFL;
				OctreeNode<T, U>* m_pLBL;
				OctreeNode<T, U>* m_pRBL;
			};
		};

		U m_min = U(FLT_MAX, FLT_MAX, FLT_MAX);
		U m_center = U();
		U m_max = -U(FLT_MAX, FLT_MAX, FLT_MAX);
	};

	template<typename T, typename U>
	class Octree
	{
	public:
		Octree(const U& min, const U& max, int maxLevel);
		~Octree();

		void Insert(T* pT, const U& position);
		
		T* Find(const U& position);
		
		void Erase(T* pT, const U& position);

		inline OctreeNode<T, U>* GetRootNode() const { return m_pRootNode; }

		inline const U& GetMin() const { if (m_pRootNode) return m_pRootNode->GetMin(); }
		inline const U& GetCenter() const { if (m_pRootNode) return m_pRootNode->GetCenter(); }
		inline const U& GetMax() const { if (m_pRootNode) return m_pRootNode->GetMax(); }

		inline void GetAllElements(set<T*>& elements) { m_pRootNode->GetAllElements(elements); }

	private:
		OctreeNode<T, U>* m_pRootNode = nullptr;
		int m_maxLevel = 5;
	};
}

#include "Octree.hpp"

