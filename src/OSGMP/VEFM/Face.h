#pragma once

#include "VEFM_Common.h"

namespace VEFM
{
	template<typename T>
	class Face : public VEFMElement
	{
	public:
		inline Vertex<T>* GetVertex0() { return m_pV0; }
		inline Vertex<T>* GetVertex1() { return m_pV1; }
		inline Vertex<T>* GetVertex2() { return m_pV2; }

		inline Vertex<T>* V0() { return m_pV0; }
		inline Vertex<T>* V1() { return m_pV1; }
		inline Vertex<T>* V2() { return m_pV2; }

		inline Edge<T>* GetEdge0() { return m_pE0; }
		inline Edge<T>* GetEdge1() { return m_pE1; }
		inline Edge<T>* GetEdge2() { return m_pE2; }

		inline Edge<T>* E0() { return m_pE0; }
		inline Edge<T>* E1() { return m_pE1; }
		inline Edge<T>* E2() { return m_pE2; }

		inline const T& GetFaceNormal() const { return m_faceNormal; }
		inline const T& GetFaceCenter() const { return m_faceCenter; }

		Edge<T>* GetCommonEdge(Face<T>* pOther);

	private:
		Face(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2);
		~Face();
		virtual void OnQueryDelete();
		
		Vertex<T>* m_pV0 = nullptr;
		Vertex<T>* m_pV1 = nullptr;
		Vertex<T>* m_pV2 = nullptr;

		Edge<T>* m_pE0 = nullptr;
		Edge<T>* m_pE1 = nullptr;
		Edge<T>* m_pE2 = nullptr;

		T m_faceNormal;
		T m_faceCenter;

	public:
		template<typename T> friend class Mesh;
	};
}

#include "Face.hpp"

