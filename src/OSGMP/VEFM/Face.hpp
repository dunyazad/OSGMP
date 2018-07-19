#pragma once

namespace VEFM
{
	template<typename T>
	Face<T>::Face(Edge<T>* pE0, Edge<T>* pE1, Edge<T>* pE2)
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

	template<typename T>
	Face<T>::~Face()
	{
		m_pE0->RemoveFaceReference(this);
		m_pE1->RemoveFaceReference(this);
		m_pE2->RemoveFaceReference(this);
	}

	template<typename T>
	Edge<T>* Face<T>::GetCommonEdge(Face<T>* pOther)
	{
		if (m_pE0 == pOther->m_pE0) return m_pE0;
		else if (m_pE0 == pOther->m_pE1) return m_pE0;
		else if (m_pE0 == pOther->m_pE2) return m_pE0;
		else if (m_pE1 == pOther->m_pE0) return m_pE1;
		else if (m_pE1 == pOther->m_pE1) return m_pE1;
		else if (m_pE1 == pOther->m_pE2) return m_pE1;
		else if (m_pE2 == pOther->m_pE0) return m_pE2;
		else if (m_pE2 == pOther->m_pE1) return m_pE2;
		else if (m_pE2 == pOther->m_pE2) return m_pE2;
		return nullptr;
	}

	template<typename T>
	void Face<T>::OnQueryDelete()
	{
		if (IsDeleteQueried()) return;

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
}

