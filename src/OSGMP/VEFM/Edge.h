#pragma once

#include "VEFM_Common.h"

namespace VEFM
{
	template <typename T>
	class Edge : public VEFMElement
	{
	public:
		inline void AddFaceReference(Face<T>* pFace) { if (IsDeleteQueried() == false) m_insidentFaces.insert(pFace); }
		inline void RemoveFaceReference(Face<T>* pFace) { if (IsDeleteQueried() == false) { m_insidentFaces.erase(pFace); if (m_insidentFaces.size() == 0) { QueryDelete(); } } }
		
		inline const set<Vertex<T>*>& GetVertices() const { return m_vertices; }
		inline const set<Face<T>*>& GetInsidentFaces() const { return m_insidentFaces; }

		Vertex<T>* GetCommonVertex(Edge<T>* pOther)
		{
			auto pV0 = *(m_vertices.begin());
			auto pV1 = *(m_vertices.rbegin());
			auto pOV0 = *(pOther->m_vertices.begin());
			auto pOV1 = *(pOther->m_vertices.rbegin());

			if (pV0 == pOV0) return pV0;
			else if (pV0 == pOV1) return pV0;
			else if (pV1 == pOV0) return pV1;
			else if (pV1 == pOV1) return pV1;
			else return nullptr;
		}

	private:
		Edge(const set<Vertex<T>*>& vertices)
			: m_vertices(vertices)
		{
			for (auto& pV : m_vertices)
			{
				pV->AddEdgeReference(this);
			}
		}

		Edge(Vertex<T>* pV0, Vertex<T>* pV1)
		{
			m_vertices.insert(pV0);
			m_vertices.insert(pV1);

			for (auto& pV : m_vertices)
			{
				pV->AddEdgeReference(this);
			}
		}

		~Edge()
		{
		}

		virtual void OnQueryDelete()
		{
			for (auto& pV : m_vertices)
			{
				if (pV != nullptr)
				{
					if (pV->IsDeleteQueried() == false)
					{
						pV->RemoveEdgeReference(this);
					}
				}
			}

			for (auto& pF : m_insidentFaces)
			{
				if (pF != nullptr)
				{
					if (pF->IsDeleteQueried() == false)
					{
						pF->QueryDelete();
					}
				}
			}
		}

		set<Vertex<T>*> m_vertices;
		set<Face<T>*> m_insidentFaces;

	public:
		template<typename T> friend class Mesh;
	};
}
