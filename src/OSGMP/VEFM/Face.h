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


		inline const Edge<T>* GetEdge0() const { return m_pE0; }
		inline const Edge<T>* GetEdge1() const { return m_pE1; }
		inline const Edge<T>* GetEdge2() const { return m_pE2; }

		inline const Edge<T>* E0() const { return m_pE0; }
		inline const Edge<T>* E1() const { return m_pE1; }
		inline const Edge<T>* E2() const { return m_pE2; }

	private:
		Face(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
		{
			m_pV0 = pE0->GetCommonVertex(pE1);
			m_pV1 = pE1->GetCommonVertex(pE2);
			m_pV2 = pE2->GetCommonVertex(pE0);

			m_pE0 = pE0;
			m_pE1 = pE1;
			m_pE2 = pE2;

			m_pE0->AddFaceReference(this);
			m_pE1->AddFaceReference(this);
			m_pE2->AddFaceReference(this);

			auto v01 = m_pV1->GetPosition() - m_pV0->GetPosition();
			auto v02 = m_pV2->GetPosition() - m_pV1->GetPosition();
			m_faceNormal = v01 ^ v02;
			m_faceNormal.normalize();

			m_faceCenter = (m_pV0->GetPosition() + m_pV1->GetPosition() + m_pV2->GetPosition()) / 3;
		}

		~Face()
		{
			m_pE0->RemoveFaceReference(this);
			m_pE1->RemoveFaceReference(this);
			m_pE2->RemoveFaceReference(this);
		}

		virtual void OnQueryDelete()
		{
			if (m_pE0 != nullptr)
			{
				if (m_pE0->IsDeleteQueried() == false)
				{
					m_pE0->RemoveFaceReference(this);
				}
			}
			if (m_pE1 != nullptr)
			{
				if (m_pE1->IsDeleteQueried() == false)
				{
					m_pE1->RemoveFaceReference(this);
				}
			}
			if (m_pE2 != nullptr)
			{
				if (m_pE2->IsDeleteQueried() == false)
				{
					m_pE2->RemoveFaceReference(this);
				}
			}
		}

		Vertex<T>* m_pV0 = nullptr;
		Vertex<T>* m_pV1 = nullptr;
		Vertex<T>* m_pV2 = nullptr;

		Edge<T>* m_pE0 = nullptr;
		Edge<T>* m_pE1 = nullptr;
		Edge<T>* m_pE2 = nullptr;

		V3 m_faceNormal;
		V3 m_faceCenter;

	public:
		template<typename T> friend class Mesh;
	};
}
