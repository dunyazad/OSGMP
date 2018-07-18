#pragma once

#include "PrimitiveRenderer.h"
#include "./VEFM/VEFM.h"
using namespace VEFM;

#define V3toVec3(v) Vec3(v.x, v.y, v.z)

template<typename T>
class PrimitiveRendererVEFM : public PrimitiveRenderer
{
public:
	PrimitiveRendererVEFM(Group* pParent)
		: PrimitiveRenderer(pParent)
	{
	}

	virtual ~PrimitiveRendererVEFM()
	{
	}

	void AddLineByEdge(const Edge<T>* pEdge, const Vec4& color0, const Vec4& color1, const string& group = "")
	{
		auto vertices = pEdge->GetVertices();
		auto vi = vertices.begin();
		auto pV0 = *vi;
		vi++;
		auto pV1 = *vi;

		PrimitiveRenderer::AddLine(pV0->P(), pV1->P(), color0, color1, group);
	}

	void AddTriangleByFace(Face<T>* pFace, const Vec4& color, bool isWireFrame, const string& group = "")
	{
		PrimitiveRenderer::AddTriangle(pFace->GetVertex0()->P(), pFace->GetVertex1()->P(), pFace->GetVertex2()->P(), color, isWireFrame, group);
	}
};
