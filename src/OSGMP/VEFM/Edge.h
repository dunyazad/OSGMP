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

		Vertex<T>* GetCommonVertex(Edge<T>* pOther);

	private:
		Edge(const set<Vertex<T>*>& vertices);
		Edge(Vertex<T>* pV0, Vertex<T>* pV1);
		~Edge();

		virtual void OnQueryDelete();

		set<Vertex<T>*> m_vertices;
		set<Face<T>*> m_insidentFaces;

	public:
		template<typename T> friend class Mesh;
	};
}

#include "Edge.hpp"

