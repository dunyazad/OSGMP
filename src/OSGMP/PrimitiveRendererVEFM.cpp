#include "PrimitiveRendererVEFM.h"

//PrimitiveRendererVEFM::PrimitiveRendererVEFM(Group* pParent)
//	: PrimitiveRenderer(pParent)
//{
//}
//
//PrimitiveRendererVEFM::~PrimitiveRendererVEFM()
//{
//}
//
//void PrimitiveRendererVEFM::AddLineByEdge(const Edge* pEdge, const Vec4& color0, const Vec4& color1, const string& group)
//{
//	auto vertices = pEdge->GetVertices();
//	auto vi = vertices.begin();
//	auto pV0 = *vi;
//	vi++;
//	auto pV1 = *vi;
//
//	PrimitiveRenderer::AddLine(V3toVec3(pV0->P()), V3toVec3(pV1->P()), color0, color1, group);
//}
//
//void PrimitiveRendererVEFM::AddTriangleByFace(Face* pFace, const Vec4& color, bool isWireFrame, const string& group)
//{
//	PrimitiveRenderer::AddTriangle(V3toVec3(pFace->GetVertex0()->P()), V3toVec3(pFace->GetVertex1()->P()), V3toVec3(pFace->GetVertex2()->P()), color, isWireFrame, group);
//}
//
//void PrimitiveRendererVEFM::AddRectangleByV3(const V3& v0, const V3& v1, const V3& v2, const V3& v3, const Vec4& color, bool isWireFrame, const string& group)
//{
//	PrimitiveRenderer::AddRectangle(V3toVec3(v0), V3toVec3(v1), V3toVec3(v2), V3toVec3(v3), color, isWireFrame, group);
//}
//
//void PrimitiveRendererVEFM::AddSphereByV3(const V3& position, float radius, const Vec4& color, bool isWireFrame, const string& group)
//{
//	PrimitiveRenderer::AddSphere(V3toVec3(position), radius, color, isWireFrame, group);
//}
//
//void PrimitiveRendererVEFM::AddBoxByV3(const V3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame, const string& group)
//{
//	PrimitiveRenderer::AddBox(V3toVec3(position), lengthX, lengthY, lengthZ, color, isWireFrame, group);
//}
