#pragma once

namespace VEFM
{
	template <typename T>
	Vertex<T>* Edge<T>::GetCommonVertex(Edge<T>* pOther)
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

	template <typename T>
	Edge<T>::Edge(const set<Vertex<T>*>& vertices)
		: m_vertices(vertices)
	{
		for (auto& pV : m_vertices)
		{
			pV->AddEdgeReference(this);
		}
	}

	template <typename T>
	Edge<T>::Edge(Vertex<T>* pV0, Vertex<T>* pV1)
	{
		m_vertices.insert(pV0);
		m_vertices.insert(pV1);

		for (auto& pV : m_vertices)
		{
			pV->AddEdgeReference(this);
		}
	}

	template <typename T>
	Edge<T>::~Edge()
	{
	}

	template <typename T>
	void Edge<T>::OnQueryDelete()
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
}
