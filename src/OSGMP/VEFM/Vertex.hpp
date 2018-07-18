#pragma once

#include "VEFM_Common.h"

namespace VEFM
{
	template<typename T>
	Vertex<T>::Vertex()
	{

	}

	template<typename T>
	Vertex<T>::Vertex(const T& position)
		: m_position(position)
	{

	}

	template<typename T>
	Vertex<T>::Vertex(float x, float y, float z)
		: m_position(x, y, z)
	{

	}

	template<typename T>
	Vertex<T>::~Vertex()
	{
	}

	template<typename T>
	void Vertex<T>::OnQueryDelete()
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
}
