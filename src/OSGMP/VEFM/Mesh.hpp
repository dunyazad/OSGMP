#pragma once

namespace VEFM
{
	template <typename T>
	Mesh<T>::Mesh()
	{
	}

	template <typename T>
	Mesh<T>::~Mesh()
	{
		if (m_pVolumeInfo != nullptr)
		{
			delete m_pVolumeInfo;
			m_pVolumeInfo = nullptr;
		}
	}

	template <typename T>
	void Mesh<T>::Clear()
	{
		if (m_pVolumeInfo != nullptr)
		{
			delete m_pVolumeInfo;
			m_pVolumeInfo = nullptr;
		}
	}

	template <typename T>
	Vertex<T>* Mesh<T>::GetVertex(const T& position)
	{
		//Vertex<T>* pVertex = nullptr;

		//if (m_vertices.count(position) != 0)
		//{
		//	return m_vertices[position];
		//}
		//else
		//{
		//	return nullptr;
		//}
		if (m_pVolumeInfo == nullptr)
		{
			m_pVolumeInfo = new Octree<Vertex<T>, T>(m_min, m_max, m_volumeInfoMaxLevel);
		}
		auto pVertex = m_pVolumeInfo->Find(position);
		if (pVertex != nullptr)
		{
			if (pVertex->IsDeleteQueried())
			{
				return nullptr;
			}
		}
		return pVertex;
	}

	template <typename T>
	Vertex<T>* Mesh<T>::GetVertex(float x, float y, float z)
	{
		return GetVertex(T(x, y, z));
	}

	template <typename T>
	Vertex<T>* Mesh<T>::GetOrCreateVertex(const T& position)
	{
		auto pVertex = GetVertex(position);

		if (pVertex != nullptr)
		{
			return pVertex;
		}
		else
		{
			pVertex = new Vertex<T>(position);
			m_pVolumeInfo->Insert(pVertex, position);
			//m_vertices[position] = pVertex;

			MIN_Vec3(m_min, position);
			MAX_Vec3(m_max, position);
			m_center = (m_min + m_max) * 0.5f;

			return pVertex;
		}
	}

	template <typename T>
	Vertex<T>* Mesh<T>::GetOrCreateVertex(float x, float y, float z)
	{
		return GetOrCreateVertex(T(x, y, z));
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetEdge(Vertex<T>* pV0, Vertex<T>* pV1)
	{
		auto& ie0 = pV0->GetIncidentEdges();
		auto& ie1 = pV1->GetIncidentEdges();

		set<Edge<T>*> Edges;
		for (auto& pE0 : ie0)
		{
			for (auto& pE1 : ie1)
			{
				if (pE0 == pE1)
				{
					Edges.insert(pE0);
				}
			}
		}

		if (Edges.size() != 0)
		{
			return *Edges.begin();
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetEdge(const T& p0, const T& p1)
	{
		auto pV0 = GetVertex(p0);
		auto pV1 = GetVertex(p1);

		if (pV0 == nullptr)
		{
			return nullptr;
		}
		else if (pV0->IsDeleteQueried())
		{
			return nullptr;
		}
		if (pV1 == nullptr)
		{
			return nullptr;
		}
		else if (pV1->IsDeleteQueried())
		{
			return nullptr;
		}

		return GetEdge(pV0, pV1);
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetEdge(float x0, float y0, float z0, float x1, float y1, float z1)
	{
		return GetEdge(T(x0, y0, z0), T(x1, y1, z1));
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetOrCreateEdge(Vertex<T>* pV0, Vertex<T>* pV1)
	{
		auto pEdge = GetEdge(pV0, pV1);

		if (pEdge != nullptr)
		{
			return pEdge;
		}
		else
		{
			pEdge = new Edge<T>(pV0, pV1);
			m_edges.push_back(pEdge);
			return pEdge;
		}
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetOrCreateEdge(const T& p0, const T& p1)
	{
		auto pV0 = GetOrCreateVertex(p0);
		auto pV1 = GetOrCreateVertex(p1);

		return GetOrCreateEdge(pV0, pV1);
	}

	template <typename T>
	Edge<T>* Mesh<T>::GetOrCreateEdge(float x0, float y0, float z0, float x1, float y1, float z1)
	{
		return GetOrCreateEdge(T(x0, y0, z0), T(x1, y1, z1));
	}

	template <typename T>
	Face<T>* Mesh<T>::GetFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
	{
		auto& if0 = pE0->GetInsidentFaces();
		auto& if1 = pE1->GetInsidentFaces();
		auto& if2 = pE2->GetInsidentFaces();

		set<Face<T>*> Faces;
		for (auto& pF0 : if0)
		{
			for (auto& pF1 : if1)
			{
				if (pF0 == pF1)
				{
					for (auto& pF2 : if2)
					{
						if (pF0 == pF1 && pF0 == pF2)
						{
							Faces.insert(pF0);
						}
					}
				}
			}
		}

		if (Faces.size() != 0)
		{
			return *Faces.begin();
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	Face<T>* Mesh<T>::GetFace(const T& p0, const T& p1, const T& p2)
	{
		auto pE0 = GetEdge(p0, p1);
		auto pE1 = GetEdge(p1, p2);
		auto pE2 = GetEdge(p2, p0);

		if (pE0 == nullptr)
		{
			return nullptr;
		}
		else if (pE0->IsDeleteQueried())
		{
			return nullptr;
		}

		if (pE1 == nullptr)
		{
			return nullptr;
		}
		else if (pE1->IsDeleteQueried())
		{
			return nullptr;
		}

		if (pE2 == nullptr)
		{
			return nullptr;
		}
		else if (pE2->IsDeleteQueried())
		{
			return nullptr;
		}

		return GetFace(pE0, pE1, pE2);
	}

	template <typename T>
	Face<T>* Mesh<T>::GetFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		return GetFace(T(x0, y0, z0), T(x1, y1, z1), T(x2, y2, z2));
	}

	template <typename T>
	Face<T>* Mesh<T>::GetOrCreateFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
	{
		auto pFace = GetFace(pE0, pE1, pE2);

		if (pFace != nullptr)
		{
			return pFace;
		}
		else
		{
			auto pFace = new Face<T>(pE0, pE1, pE2);
			m_faces.push_back(pFace);
			return pFace;
		}
	}

	template <typename T>
	Face<T>* Mesh<T>::GetOrCreateFace(const T& p0, const T& p1, const T& p2)
	{
		auto pE0 = GetOrCreateEdge(p0, p1);
		auto pE1 = GetOrCreateEdge(p1, p2);
		auto pE2 = GetOrCreateEdge(p2, p0);

		return GetOrCreateFace(pE0, pE1, pE2);
	}

	template <typename T>
	Face<T>* Mesh<T>::GetOrCreateFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		return GetOrCreateFace(T(x0, y0, z0), T(x1, y1, z1), T(x2, y2, z2));
	}

	template <typename T>
	void Mesh<T>::GetVerticesAndIndices(vector<float>& positions, vector<int>& indices)
	{
		map<T, int> positionIndexMap;
		vector<T> TPositions;

		for (auto& pF : m_faces)
		{
			if (pF->IsDeleteQueried()) continue;
			if (pF->E0()->IsDeleteQueried()) continue;
			if (pF->E1()->IsDeleteQueried()) continue;
			if (pF->E2()->IsDeleteQueried()) continue;
			if (pF->V0()->IsDeleteQueried()) continue;
			if (pF->V1()->IsDeleteQueried()) continue;
			if (pF->V2()->IsDeleteQueried()) continue;

			auto& p0 = pF->V0()->GetPosition();
			auto& p1 = pF->V1()->GetPosition();
			auto& p2 = pF->V2()->GetPosition();

			if (positionIndexMap.count(p0) == 0)
			{
				TPositions.push_back(p0);
				positionIndexMap[p0] = TPositions.size() - 1;
			}
			indices.push_back(positionIndexMap[p0]);

			if (positionIndexMap.count(p1) == 0)
			{
				TPositions.push_back(p1);
				positionIndexMap[p1] = TPositions.size() - 1;
			}
			indices.push_back(positionIndexMap[p1]);

			if (positionIndexMap.count(p2) == 0)
			{
				TPositions.push_back(p2);
				positionIndexMap[p2] = TPositions.size() - 1;
			}
			indices.push_back(positionIndexMap[p2]);
		}

		for (auto& p : TPositions)
		{
			positions.push_back(p.x());
			positions.push_back(p.y());
			positions.push_back(p.z());
		}
	}

	template <typename T>
	void Mesh<T>::Refresh()
	{
		auto& fi = m_faces.begin();
		while (fi != m_faces.end())
		{
			if ((*fi)->IsDeleteQueried())
			{
				delete *fi;
				fi = m_faces.erase(fi);
			}
			else
			{
				fi++;
			}
		}

		auto& ei = m_edges.begin();
		while (ei != m_edges.end())
		{
			if ((*ei)->IsDeleteQueried())
			{
				delete *ei;
				ei = m_edges.erase(ei);
			}
			else
			{
				ei++;
			}
		}

		stack<OctreeNode<Vertex<T>, T>*> nodeStack;
		nodeStack.push(m_pVolumeInfo->GetRootNode());
		while (!nodeStack.empty())
		{
			auto& pNode = nodeStack.top();
			nodeStack.pop();

			auto& elements = pNode->GetElements();

			auto& vi = elements.begin();
			while (vi != elements.end())
			{
				if ((*vi)->IsDeleteQueried())
				{
					delete (*vi);
					vi = elements.erase(vi);
				}
				else
				{
					vi++;
				}
			}

			auto children = pNode->GetChildren();
			for (int i = 0; i < OctreeNodeIndex::INDEX_SIZE; i++)
			{
				if (children[i] != nullptr)
				{
					nodeStack.push((OctreeNode<Vertex<T>, T>*)children[i]);
				}
			}
		}
	}

	template<typename T>
	void Mesh<T>::CollapseEdge(Edge<T>* pE)
	{
		if (pE == nullptr)
			return;
		
		set<Edge<T>*> edges;

		auto& vs = pE->GetVertices();
		auto& vsi = vs.begin();
		auto pV0 = *vsi;
		vsi++;
		auto pV1 = *vsi;

		pV0->QueryDelete();
		pV1->QueryDelete();

		for (auto& pE : pV0->GetIncidentEdges())
		{
			edges.insert(pE);
		}
		for (auto& pE : pV1->GetIncidentEdges())
		{
			edges.insert(pE);
		}

		set<Face<T>*> faces;
		for (auto& pE : edges)
		{
			for (auto& pF : pE->GetInsidentFaces())
			{
				faces.insert(pF);
			}
		}

		auto pVN = GetOrCreateVertex((pV0->P() + pV1->P()) * 0.5f);
		for (auto& pF : faces)
		{
			pF->QueryDelete();

			if (pF->E0() == pE || pF->E1() == pE || pF->E2() == pE)
			{

			}
			else if (pF->V0() == pV0 || pF->V0() == pV1)
			{
				GetOrCreateFace(pVN->P(), pF->V1()->P(), pF->V2()->P());
			}
			else if (pF->V1() == pV0 || pF->V1() == pV1)
			{
				GetOrCreateFace(pF->V0()->P(), pVN->P(), pF->V2()->P());
			}
			else if (pF->V2() == pV0 || pF->V2() == pV1)
			{
				GetOrCreateFace(pF->V0()->P(), pF->V1()->P(), pVN->P());
			}
		}
	}

	template<typename T>
	void Mesh<T>::SplitEdge(Edge<T>* pE)
	{
		if (pE == nullptr)
			return;

		//auto& faces = pE->GetInsidentFaces();

		//auto& vs = pE->GetVertices();
		//auto& vsi = vs.begin();
		//auto pV0 = *vsi;
		//vsi++;
		//auto pV1 = *vsi;

		////pvd->AddLine(pV0->P(), pV1->P(), V4_RED, V4_RED);

		//auto pVN = GetOrCreateVertex((pV0->P() + pV1->P()) * 0.5f);
		//set<tuple<T, T, T>> toCreate;
		//set<Face<T>*> toDelete;
		//for (auto& pF : faces)
		//{
		//	if (pV0 == pF->V0() && pV1 == pF->V1())
		//	{
		//		toCreate.insert(make_tuple(pV0->P(), pVN->P(), pV2->()));
		//	}
		//	else if (pV0 == pF->V1() && pV1 == pF->V2())
		//	{
		//		toCreate.insert(make_tuple(pV0->P(), pVN->P(), pV2->()));
		//	}

		//	toDelete.insert(pF);
		//}
		//for (auto& pF : toDelete)
		//{
		//	pF->QueryDelete();
		//}
		//for (auto& vvv : toCreate)
		//{
		//	GetOrCreateFace(get<0>(vvv), get<1>(vvv), get<2>(vvv));

		//	printf("%f %f %f\n", get<0>(vvv).x(), get<0>(vvv).y(), get<0>(vvv).z());
		//	printf("%f %f %f\n", get<1>(vvv).x(), get<1>(vvv).y(), get<1>(vvv).z());
		//	printf("%f %f %f\n", get<2>(vvv).x(), get<2>(vvv).y(), get<2>(vvv).z());
		//}
	}

	template<typename T>
	void Mesh<T>::FlipEdge(Edge<T>* pE)
	{
		if (pE == nullptr)
			return;

		auto& faces = pE->GetInsidentFaces();
		if (faces.size() > 1)
		{
			auto& fi = faces.begin();
			auto pF0 = *fi;
			fi++;
			auto pF1 = *fi;

			FlipEdge(pF0, pF1, pE);
		}
	}

	template<typename T>
	void Mesh<T>::FlipEdge(Face<T>* pF0, Face<T>* pF1)
	{
		if (pF0 == nullptr || pF1 == nullptr)
			return;

		auto pE = pF0->GetCommonEdge(pF1);
		FlipEdge(pF0, pF1, pE);
	}

	template <typename T>
	void Mesh<T>::FlipEdge(Face<T>* pF0, Face<T>* pF1, Edge<T>* pE)
	{
		if (pF0 == nullptr || pF1 == nullptr || pE == nullptr)
			return;

		pF0->QueryDelete();
		pF1->QueryDelete();

		Vertex<T>* pCV0 = nullptr;
		Vertex<T>* pCV1 = nullptr;
		Vertex<T>* pNV0 = nullptr;
		Vertex<T>* pNV1 = nullptr;

		auto& cvs = pE->GetVertices();
		auto& cvsi = cvs.begin();
		pCV0 = *cvsi;
		cvsi++;
		pCV1 = *cvsi;

		if (pF0->V0() != pCV0 && pF0->V0() != pCV1) pNV0 = pF0->V0();
		else if (pF0->V1() != pCV0 && pF0->V1() != pCV1) pNV0 = pF0->V1();
		else if (pF0->V2() != pCV0 && pF0->V2() != pCV1) pNV0 = pF0->V2();
		else return;

		if (pF1->V0() != pCV0 && pF1->V0() != pCV1) pNV1 = pF1->V0();
		else if (pF1->V1() != pCV0 && pF1->V1() != pCV1) pNV1 = pF1->V1();
		else if (pF1->V2() != pCV0 && pF1->V2() != pCV1) pNV1 = pF1->V2();
		else return;

		auto& pc0 = pCV0->P();
		auto& pc1 = pCV1->P();
		auto& pn0 = pNV0->P();
		auto& pn1 = pNV1->P();

		auto d00 = pn0 - pc0;
		auto d01 = pn1 - pc0;
		auto normal = d01 ^ d00;
		normal.normalize();

		if (pF0->GetFaceNormal() * normal < 0)
		{
			GetOrCreateFace(pn1, pc0, pn0);
			GetOrCreateFace(pc1, pn1, pn0);
		}
		else
		{
			GetOrCreateFace(pn0, pc0, pn1);
			GetOrCreateFace(pn0, pn1, pc1);
		}
	}

	template <typename T>
	Edge<T>* Mesh<T>::FindFirstMetBorderEdge()
	{
		for (auto& pE : m_edges)
		{
			if (pE->GetInsidentFaces().size() == 1)
			{
				return pE;
			}
		}

		return nullptr;
	}

	template <typename T>
	vector<Edge<T>*> Mesh<T>::FindBorderEdges()
	{
		vector<Edge<T>*> result;

		for (auto& pE : m_edges)
		{
			if (pE->GetInsidentFaces().size() == 1)
			{
				result.push_back(pE);
			}
		}

		return result;
	}

	template <typename T>
	vector<Edge<T>*> Mesh<T>::FindNonManifoldEdges()
	{
		vector<Edge<T>*> result;

		for (auto& pE : m_edges)
		{
			if (pE->GetInsidentFaces().size() > 2)
			{
				result.push_back(pE);
			}
		}

		return result;
	}

	template <typename T>
	void Mesh<T>::OnQueryDelete()
	{
		for (auto& pF : m_faces)
		{
			pF->QueryDelete();
		}
	}

	template <typename T>
	void Mesh<T>::PrintInformation()
	{
		set<Vertex<T>*> vertices;
		m_pVolumeInfo->GetAllElements(vertices);

		int deleteQueriedVertices = 0;
		int liveVertices = 0;
		for (auto& pV : vertices)
		{
			if (pV->IsDeleteQueried())
			{
				deleteQueriedVertices++;
			}
			else
			{
				liveVertices++;
			}
		}

		printf("Number of Vertices : %d\n", vertices.size());
		printf("Number of Delete Queried Vertices : %d\n", deleteQueriedVertices);
		printf("Number of Live Vertices : %d\n", liveVertices);

		int deleteQueriedEdges = 0;
		int liveEdges = 0;
		for (auto& pE : m_edges)
		{
			if (pE->IsDeleteQueried())
			{
				deleteQueriedEdges++;
			}
			else
			{
				liveEdges++;
			}
		}

		printf("Number of Edges : %d\n", m_edges.size());
		printf("Number of Delete Queried Edges : %d\n", deleteQueriedEdges);
		printf("Number of Live Edges : %d\n", liveEdges);

		int deleteQueriedFaces = 0;
		int liveFaces = 0;
		for (auto& pE : m_faces)
		{
			if (pE->IsDeleteQueried())
			{
				deleteQueriedFaces++;
			}
			else
			{
				liveFaces++;
			}
		}

		printf("Number of Faces : %d\n", m_faces.size());
		printf("Number of Delete Queried Faces : %d\n", deleteQueriedFaces);
		printf("Number of Live Faces : %d\n", liveFaces);
	}
}
