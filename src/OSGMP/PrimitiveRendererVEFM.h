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

		PrimitiveRenderer::AddLine(V3toVec3(pV0->P()), V3toVec3(pV1->P()), color0, color1, group);
	}

	void AddTriangleByFace(Face<T>* pFace, const Vec4& color, bool isWireFrame, const string& group = "")
	{
		PrimitiveRenderer::AddTriangle(V3toVec3(pFace->GetVertex0()->P()), V3toVec3(pFace->GetVertex1()->P()), V3toVec3(pFace->GetVertex2()->P()), color, isWireFrame, group);
	}

	void AddRectangleByV3(const V3& v0, const V3& v1, const V3& v2, const V3& v3, const Vec4& color, bool isWireFrame, const string& group = "")
	{
		PrimitiveRenderer::AddRectangle(V3toVec3(v0), V3toVec3(v1), V3toVec3(v2), V3toVec3(v3), color, isWireFrame, group);
	}

	void AddSphereByV3(const V3& position, float radius, const Vec4& color, bool isWireFrame, const string& group = "")
	{
		PrimitiveRenderer::AddSphere(V3toVec3(position), radius, color, isWireFrame, group);
	}

	void AddBoxByV3(const V3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame, const string& group = "")
	{
		PrimitiveRenderer::AddBox(V3toVec3(position), lengthX, lengthY, lengthZ, color, isWireFrame, group);
	}
};
