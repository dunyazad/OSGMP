#pragma once

#include "VEFM_Common.h"

namespace VEFM
{
	template<typename T>
	class Vertex : public VEFMElement
	{
	public:
		inline V3& GetPosition() { return m_position; }
		//inline const V3& GetPosition() const { return m_position; }
		inline const V3& P() const { return m_position; }

		inline void AddEdgeReference(Edge<T>* pEdge) { if(IsDeleteQueried() == false) m_incidentEdges.insert(pEdge); }
		inline void RemoveEdgeReference(Edge<T>* pEdge) { if (IsDeleteQueried() == false) { m_incidentEdges.erase(pEdge); if (m_incidentEdges.size() == 0) QueryDelete(); } }

		inline const set<Edge<T>*>& GetIncidentEdges() const { return m_incidentEdges; }

	private:
		Vertex()
		{

		}

		Vertex(const V3& position)
			: m_position(position)
		{

		}

		Vertex(float x, float y, float z)
			: m_position(x, y, z)
		{

		}

		~Vertex()
		{
		}

		virtual void OnQueryDelete()
		{
			for (auto& pE : m_incidentEdges)
			{
				if (pE != nullptr)
				{
					if (pE->IsDeleteQueried() == false)
					{
						pE->QueryDelete();
					}
				}
			}
		}

		V3 m_position;
		set<Edge<T>*> m_incidentEdges;

	public:
		template<typename T> friend class Mesh;
	};
}