#pragma once

#include "../OSGCommon.h"
using namespace osg;

#include "PredefinedColor.h"

#include <string>
using namespace std;

class PrimitiveRendererGroup;
class PrimitiveRendererElements;
class PrimitiveRendererElementLines;

class PrimitiveRenderer
{
public:
	PrimitiveRenderer(Group* pParent);
	virtual ~PrimitiveRenderer();

	PrimitiveRendererGroup* GetOrCreateRenderGroup(const string& name);

	void AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1, const string& group = "");
	void AddTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec4& color, bool isWireFrame, const string& group = "");
	void AddRectangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec4& color, bool isWireFrame, const string& group = "");
	void AddSphere(const Vec3& position, float radius, const Vec4& color, bool isWireFrame, const string& group = "");
	void AddBox(const Vec3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame, const string& group = "");

	void ClearAll();
	void Clear(const string& group = "");

	void SetVisible(bool bVisible, const string& group = "");
	void ToggleVisible(const string& group = "");

protected:
	Group* m_pParent = nullptr;
	map<string, PrimitiveRendererGroup*> m_rendererGroups;
};

class PrimitiveRendererGroup
{
public:
	PrimitiveRendererGroup(Group* pParent);
	virtual ~PrimitiveRendererGroup();

	void AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1);
	void AddTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec4& color, bool isWireFrame);
	void AddRectangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec4& color, bool isWireFrame);
	void AddSphere(const Vec3& position, float radius, const Vec4& color, bool isWireFrame);
	void AddBox(const Vec3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame);

	void Clear();

	void SetVisible(bool bVisible);
	void ToggleVisible();

protected:
	Group* m_pParent = nullptr;
	Switch* m_pSwitch = nullptr;
	ref_ptr<Geode> m_pGeodeBoxes;
	ref_ptr<Geode> m_pGeodeShperes;
	ref_ptr<Geode> m_pGeodeTriangles;
	ref_ptr<Geode> m_pGeodeRectangles;
	ref_ptr<Geode> m_pGeodeLines;

	PrimitiveRendererElementLines* m_pLines;
};

class PrimitiveRendererElements
{
public:
	PrimitiveRendererElements(Group* pParent);
	~PrimitiveRendererElements();

	inline Group* GetParent() { return m_pParent; }
	inline ref_ptr<Geode> GetGeode() { return m_pGeode; }
	inline ref_ptr<Geometry> GetGeometry() { return m_pGeometry; }
	inline ref_ptr<Vec3Array> GetVertexArray() { return m_pVertexArray; }
	inline ref_ptr<DrawElementsUInt> GetIndices() { return m_pIndices; }
	inline ref_ptr<Vec3Array> GetNormalArray() { return m_pNormalArray; }
	inline ref_ptr<Vec4Array> GetColorArray() { return m_pColorArray; }

protected:
	Group * m_pParent;
	ref_ptr<Geode> m_pGeode;
	ref_ptr<Geometry> m_pGeometry;
	ref_ptr<Vec3Array> m_pVertexArray;
	ref_ptr<DrawElementsUInt> m_pIndices;
	ref_ptr<Vec3Array> m_pNormalArray;
	ref_ptr<Vec4Array> m_pColorArray;
};

class PrimitiveRendererElementLines : public PrimitiveRendererElements
{
public:
	PrimitiveRendererElementLines(Group* pParent);
	~PrimitiveRendererElementLines();

	void AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1);
	void Clear();
};
