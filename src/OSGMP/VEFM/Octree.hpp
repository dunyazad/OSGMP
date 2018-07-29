#pragma once

namespace VEFM
{
	//bool PointOnTriangle(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c, bool bIncludeEdge)
	//{
	//	auto v0 = c - a;
	//	auto v1 = b - a;
	//	auto v2 = p - a;

	//	auto dot00 = v0 * v0;
	//	auto dot01 = v0 * v1;
	//	auto dot02 = v0 * v2;
	//	auto dot11 = v1 * v1;
	//	auto dot12 = v1 * v2;

	//	auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	//	auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	//	auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	//	// Check if point is in triangle
	//	if (bIncludeEdge)
	//	{
	//		return (u >= 0) && (v >= 0) && (u + v <= 1);
	//	}
	//	else
	//	{
	//		return (u > 0) && (v > 0) && (u + v < 1);
	//	}
	//}

	//void Triangluate(const Vec3& fv0, const Vec3& fv1, const Vec3& fv2, const vector<Vec3>& points, vector<tuple<Vec3, Vec3, Vec3>>& result)
	//{
	//	auto d01 = fv1 - fv0;
	//	auto d02 = fv2 - fv0;
	//	auto normal = d02 ^ d01;
	//	normal.normalize();
	//	auto center = (fv0 + fv1 + fv2) / 3;

	//	Matrix m;
	//	m.makeRotate(Vec3(0, 0, 1), normal);
	//	m.translate(center);

	//	auto im = Matrix::inverse(m);

	//	//ref_ptr<Vec3Array> pVertices = new Vec3Array;

	//	//auto pfv0 = im * fv0;
	//	//pVertices->push_back(Vec3(pfv0.x(), pfv0.y(), 0));
	//	//auto pfv1 = im * fv1;
	//	//pVertices->push_back(Vec3(pfv1.x(), pfv1.y(), 0));
	//	//auto pfv2 = im * fv2;
	//	//pVertices->push_back(Vec3(pfv2.x(), pfv2.y(), 0));

	//	//for (auto& p : points)
	//	//{
	//	//	auto pp = im * p;
	//	//	pVertices->push_back(Vec3(pp.x(), pp.y(), 0));
	//	//}

	//	vector<Vec3> vertices;

	//	auto pfv0 = fv0 * im;
	//	vertices.push_back(Vec3(pfv0.x(), pfv0.y(), 0));
	//	auto pfv1 = fv1 * im;
	//	vertices.push_back(Vec3(pfv1.x(), pfv1.y(), 0));
	//	auto pfv2 = fv2 * im;
	//	vertices.push_back(Vec3(pfv2.x(), pfv2.y(), 0));

	//	for (auto& p : points)
	//	{
	//		auto pp = p * im;
	//		vertices.push_back(Vec3(pp.x(), pp.y(), 0));
	//	}




	//	//ref_ptr<osgUtil::DelaunayTriangulator> pDT = new osgUtil::DelaunayTriangulator;
	//	//pDT->setInputPointArray(pVertices);
	//	//pDT->triangulate();

	//	//auto pTriangles = pDT->getTriangles();
	//	//for (unsigned int i = 0; i < pTriangles->size() / 3; i++)
	//	//{
	//	//	auto i0 = pTriangles->at(i * 3 + 0);
	//	//	auto i1 = pTriangles->at(i * 3 + 1);
	//	//	auto i2 = pTriangles->at(i * 3 + 2);
	//	//	auto& v0 = pVertices->at(i0);
	//	//	auto& v1 = pVertices->at(i1);
	//	//	auto& v2 = pVertices->at(i2);

	//	//	result.push_back(make_tuple(m * v0, m * v1, m * v2));
	//	//}
	//}

	template<typename T, typename U>
	OctreeNode<T, U>::OctreeNode(const OctreeNode<T, U>* pParent, int level, const U& min, const U& max)
		: m_pParent(pParent), m_level(level), m_min(min), m_center((min + max) * 0.5f), m_max(max)
	{
		for (int i = 0; i < INDEX_SIZE; i++)
		{
			m_children[i] = nullptr;
		}
	}

	template<typename T, typename U>
	OctreeNode<T, U>::~OctreeNode()
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

	template<typename T, typename U>
	OctreeNode<T, U>* OctreeNode<T, U>::GetChild(OctreeNodeIndex index)
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

	template<typename T, typename U>
	OctreeNode<T, U>* OctreeNode<T, U>::GetOrCreateChild(OctreeNodeIndex index)
	{
		auto pChild = GetChild(index);
		if (pChild != nullptr)
		{
			return pChild;
		}
		else
		{
			m_children[index] = new OctreeNode<T, U>(this);
			return m_children[index];
		}
	}

	template<typename T, typename U>
	void OctreeNode<T, U>::GetAllElements(set<T*>& elements)
	{
		for (int i = 0; i < OctreeNodeIndex::INDEX_SIZE; i++)
		{
			if (m_children[i] != nullptr)
			{
				m_children[i]->GetAllElements(elements);
			}
		}

		for (auto& t : m_elements)
		{
			elements.insert(t);
		}
	}

	template<typename T, typename U>
	void OctreeNode<T, U>::Insert(T* pT, const U& position, int maxLevel)
	{
		if (m_level != maxLevel)
		{
			auto index = DetermineIndex(position);
			if (m_children[index] == nullptr)
			{
				U min;
				U max;
				GetMinMaxByIndex(index, min, max);
				m_children[index] = new OctreeNode<T, U>(this, m_level + 1, min, max);
			}
			m_children[index]->Insert(pT, position, maxLevel);
		}
		else
		{
			m_elements.insert(pT);
		}
	}

	template<typename T, typename U>
	OctreeNodeIndex OctreeNode<T, U>::DetermineIndex(const U& position)
	{
		if (position.x() <= m_center.x())
			if (position.y() <= m_center.y())
				if (position.z() <= m_center.z()) return OctreeNodeIndex::LFL;
				else return OctreeNodeIndex::LFU;
			else
				if (position.z() <= m_center.z()) return OctreeNodeIndex::LBL;
				else return OctreeNodeIndex::LBU;
		else
			if (position.y() <= m_center.y())
				if (position.z() <= m_center.z()) return OctreeNodeIndex::RFL;
				else return OctreeNodeIndex::RFU;
			else
				if (position.z() <= m_center.z()) return OctreeNodeIndex::RBL;
				else return OctreeNodeIndex::RBU;
	}

	template<typename T, typename U>
	void OctreeNode<T, U>::GetMinMaxByIndex(OctreeNodeIndex index, U& min, U& max)
	{
		switch (index)
		{
		case OctreeNodeIndex::LFL:
			min = m_min;
			max = m_center;
			break;
		case OctreeNodeIndex::LFU:
			min.x() = m_min.x();
			min.y() = m_min.y();
			min.z() = m_center.z();
			max.x() = m_center.x();
			max.y() = m_center.y();
			max.z() = m_max.z();
			break;
		case OctreeNodeIndex::LBL:
			min.x() = m_min.x();
			min.y() = m_center.y();
			min.z() = m_min.z();
			max.x() = m_center.x();
			max.y() = m_max.y();
			max.z() = m_center.z();
			break;
		case OctreeNodeIndex::LBU:
			min.x() = m_min.x();
			min.y() = m_center.y();
			min.z() = m_center.z();
			max.x() = m_center.x();
			max.y() = m_max.y();
			max.z() = m_max.z();
			break;
		case OctreeNodeIndex::RFL:
			min.x() = m_center.x();
			min.y() = m_min.y();
			min.z() = m_min.z();
			max.x() = m_max.x();
			max.y() = m_center.y();
			max.z() = m_center.z();
			break;
		case OctreeNodeIndex::RFU:
			min.x() = m_center.x();
			min.y() = m_min.y();
			min.z() = m_center.z();
			max.x() = m_max.x();
			max.y() = m_center.y();
			max.z() = m_max.z();
			break;
		case OctreeNodeIndex::RBL:
			min.x() = m_center.x();
			min.y() = m_center.y();
			min.z() = m_min.z();
			max.x() = m_max.x();
			max.y() = m_max.y();
			max.z() = m_center.z();
			break;
		case OctreeNodeIndex::RBU:
			min = m_center;
			max = m_max;
			break;
		default:
			break;
		}
	}

	template<typename T, typename U>
	bool OctreeNode<T, U>::ChildExists() const
	{
		for (int i = 0; i < INDEX_SIZE; i++)
		{
			if (m_children[i] != nullptr)
				return true;
		}
		return false;
	}

	template<typename T, typename U>
	T* OctreeNode<T, U>::Find(const U& position, int maxLevel)
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
			auto& vi = m_elements.begin();
			while (vi != m_elements.end())
			{
				auto& pV = *vi;
				if (pV->IsDeleteQueried())
				{
					vi = m_elements.erase(vi);
				}
				else
				{
					//if ((pV->P() - position).length() < EPSILON)
					if ((pV->P() - position).length() < 0.00001f)
					{
						return pV;
					}
					vi++;
				}
			}

			return nullptr;
		}
	}

	template<typename T, typename U>
	void OctreeNode<T, U>::Erase(T* pT, const U& position, int maxLevel)
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

	template<typename T, typename U>
	Octree<T, U>::Octree(const U& min, const U& max, int maxLevel)
		: m_maxLevel(maxLevel)
	{
		m_pRootNode = new OctreeNode<T, U>(nullptr, 0, min, max);
	}

	template<typename T, typename U>
	Octree<T, U>::~Octree()
	{
		if (m_pRootNode != nullptr)
		{
			delete m_pRootNode;
			m_pRootNode = nullptr;
		}
	}

	template<typename T, typename U>
	void Octree<T, U>::Insert(T* pT, const U& position)
	{
		m_pRootNode->Insert(pT, position, m_maxLevel);
	}

	template<typename T, typename U>
	T* Octree<T, U>::Find(const U& position)
	{
		return m_pRootNode->Find(position, m_maxLevel);
	}

	template<typename T, typename U>
	void Octree<T, U>::Erase(T* pT, const U& position)
	{
		return m_pRootNode->Erase(pT, position, m_maxLevel);
	}
}
