#pragma once

#include "VEFM_Common.h"

#include "Octree.h"

namespace VEFM
{
	template <typename T>
	class Mesh : public VEFMElement
	{
	public:
		Mesh()
		{
		}

		~Mesh()
		{
			if (m_pVolumeInfo != nullptr)
			{
				delete m_pVolumeInfo;
				m_pVolumeInfo = nullptr;
			}
		}

		void Clear()
		{
			if (m_pVolumeInfo != nullptr)
			{
				delete m_pVolumeInfo;
				m_pVolumeInfo = nullptr;
			}
		}

		Vertex<T>* GetVertex(const V3& position)
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
				m_pVolumeInfo = new Octree<Vertex<T>>(m_min, m_max, m_volumeInfoMaxLevel);
			}
			return m_pVolumeInfo->Find(position);
		}

		Vertex<T>* GetVertex(float x, float y, float z)
		{
			return GetVertex(V3(x, y, z));
		}

		Vertex<T>* GetOrCreateVertex(const V3& position)
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

				MIN_V3(m_min, position);
				MAX_V3(m_max, position);
				m_center = (m_min + m_max) * 0.5f;

				return pVertex;
			}
		}

		Vertex<T>* GetOrCreateVertex(float x, float y, float z)
		{
			return GetOrCreateVertex(V3(x, y, z));
		}

		Vertex<T>* GetNearestVertex(const V3& position)
		{
			return m_pVolumeInfo->FindNearest(position);
		}

		Vertex<T>* GetNearestVertex(float x, float y, float z)
		{
			return GetNearestVertex(V3(x, y, z));
		}

		Edge<T>* GetEdge(Vertex<T>* pV0, Vertex<T>* pV1)
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

		Edge<T>* GetEdge(const V3& p0, const V3& p1)
		{
			auto pV0 = GetVertex(p0);
			auto pV1 = GetVertex(p1);

			if (pV0 == nullptr || pV1 == nullptr)
			{
				return nullptr;
			}
			else
			{
				return GetEdge(pV0, pV1);
			}
		}

		Edge<T>* GetEdge(float x0, float y0, float z0, float x1, float y1, float z1)
		{
			return GetEdge(V3(x0, y0, z0), V3(x1, y1, z1));
		}

		Edge<T>* GetOrCreateEdge(Vertex<T>* pV0, Vertex<T>* pV1)
		{
			auto pEdge = GetEdge(pV0, pV1);

			if (pEdge != nullptr)
			{
				return pEdge;
			}
			else
			{
				pEdge = new Edge<T>(pV0, pV1);
				m_Edges.push_back(pEdge);
				return pEdge;
			}
		}

		Edge<T>* GetOrCreateEdge(const V3& p0, const V3& p1)
		{
			auto pV0 = GetOrCreateVertex(p0);
			auto pV1 = GetOrCreateVertex(p1);

			return GetOrCreateEdge(pV0, pV1);
		}

		Edge<T>* GetOrCreateEdge(float x0, float y0, float z0, float x1, float y1, float z1)
		{
			return GetOrCreateEdge(V3(x0, y0, z0), V3(x1, y1, z1));
		}

		Face<T>* GetFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
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

		Face<T>* GetFace(const V3& p0, const V3& p1, const V3& p2)
		{
			auto pE0 = GetEdge(p0, p1);
			auto pE1 = GetEdge(p1, p2);
			auto pE2 = GetEdge(p2, p0);

			if (pE0 == nullptr || pE1 == nullptr || pE2 == nullptr)
			{
				return nullptr;
			}
			else
			{
				return GetFace(pE0, pE1, pE2);
			}
		}

		Face<T>* GetFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
		{
			return GetFace(V3(x0, y0, z0), V3(x1, y1, z1), V3(x2, y2, z2));
		}

		Face<T>* GetOrCreateFace(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
		{
			auto pFace = GetFace(pE0, pE1, pE2);

			if (pFace != nullptr)
			{
				return pFace;
			}
			else
			{
				auto pFace = new Face<T>(pE0, pE1, pE2);
				m_Faces.push_back(pFace);
				return pFace;
			}
		}

		Face<T>* GetOrCreateFace(const V3& p0, const V3& p1, const V3& p2)
		{
			auto pE0 = GetOrCreateEdge(p0, p1);
			auto pE1 = GetOrCreateEdge(p1, p2);
			auto pE2 = GetOrCreateEdge(p2, p0);

			return GetOrCreateFace(pE0, pE1, pE2);
		}

		Face<T>* GetOrCreateFace(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
		{
			return GetOrCreateFace(V3(x0, y0, z0), V3(x1, y1, z1), V3(x2, y2, z2));
		}

		void GetVerticesAndIndices(vector<float>& positions, vector<int>& indices)
		{
			map<V3, int> positionIndexMap;
			vector<V3> v3Positions;

			for (auto& pF : m_Faces)
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
					v3Positions.push_back(p0);
					positionIndexMap[p0] = v3Positions.size() - 1;
				}
				indices.push_back(positionIndexMap[p0]);

				if (positionIndexMap.count(p1) == 0)
				{
					v3Positions.push_back(p1);
					positionIndexMap[p1] = v3Positions.size() - 1;
				}
				indices.push_back(positionIndexMap[p1]);

				if (positionIndexMap.count(p2) == 0)
				{
					v3Positions.push_back(p2);
					positionIndexMap[p2] = v3Positions.size() - 1;
				}
				indices.push_back(positionIndexMap[p2]);
			}

			for (auto& p : v3Positions)
			{
				positions.push_back(p.x);
				positions.push_back(p.y);
				positions.push_back(p.z);
			}
		}

		void Refresh()
		{
			auto& fi = m_Faces.begin();
			while (fi != m_Faces.end())
			{
				if ((*fi)->IsDeleteQueried())
				{
					delete *fi;
					fi = m_Faces.erase(fi);
				}
				else
				{
					fi++;
				}
			}

			auto& ei = m_Edges.begin();
			while (ei != m_Edges.end())
			{
				if ((*ei)->IsDeleteQueried())
				{
					delete *ei;
					ei = m_Edges.erase(ei);
				}
				else
				{
					ei++;
				}
			}

			stack<OctreeNode<Vertex<T>>*> nodeStack;
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
				for (int i = 0; i < OctreeNode<Vertex<T>>::INDEX_SIZE; i++)
				{
					if (children[i] != nullptr)
					{
						nodeStack.push((OctreeNode<Vertex<T>>*)children[i]);
					}
				}
			}
		}

		Edge<T>* FindFirstMetBorderEdge()
		{
			for (auto& pE : m_Edges)
			{
				if (pE->GetInsidentFaces().size() == 1)
				{
					return pE;
				}
			}

			return nullptr;
		}

		vector<Edge<T>*> FindBorderEdges()
		{
			vector<Edge<T>*> result;

			for (auto& pE : m_Edges)
			{
				if (pE->GetInsidentFaces().size() == 1)
				{
					result.push_back(pE);
				}
			}

			return result;
		}

		vector<Edge<T>*> FindNonManifoldEdges()
		{
			vector<Edge<T>*> result;

			for (auto& pE : m_Edges)
			{
				if (pE->GetInsidentFaces().size() > 2)
				{
					result.push_back(pE);
				}
			}

			return result;
		}

		//inline const map<V3, Vertex<T>*>& GetVertices() const { return m_vertices; }
		inline const vector<Edge<T>*>& GetEdges() const { return m_Edges; }
		inline const vector<Face<T>*>& GetFaces() const { return m_Faces; }

		inline const V3& GetMin() const { return m_min; }
		inline const V3& GetCenter() const { return m_center; }
		inline const V3& GetMax() const { return m_max; }

		inline void SetVolumeMinMax(const V3& min, const V3& max) { m_min = min; m_max = max; m_center = (min + max) * 0.5f; }
		inline Octree<Vertex<T>>* GetVolumeInfo() const { return m_pVolumeInfo; }

	private:
		//map<V3, Vertex<T>*> m_vertices;
		vector<Edge<T>*> m_Edges;
		vector<Face<T>*> m_Faces;

		V3 m_min;
		V3 m_center;
		V3 m_max;

		Octree<Vertex<T>>* m_pVolumeInfo = nullptr;
		int m_volumeInfoMaxLevel = 10;

		virtual void OnQueryDelete()
		{
			for (auto& pF : m_Faces)
			{
				pF->QueryDelete();
			}
		}
	};
}
