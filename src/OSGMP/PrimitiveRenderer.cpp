#include "PrimitiveRenderer.h"

PrimitiveRenderer::PrimitiveRenderer(Group* pParent)
{
	m_pParent = pParent;

	PrimitiveRendererGroup* pRendererGroup = new PrimitiveRendererGroup(m_pParent);
	m_rendererGroups["Default"] = pRendererGroup;
}

PrimitiveRenderer::~PrimitiveRenderer()
{
	for (auto& kvp : m_rendererGroups)
	{
		kvp.second->Clear();
		delete kvp.second;
	}
}

PrimitiveRendererGroup* PrimitiveRenderer::GetOrCreateRenderGroup(const string& name)
{
	if (name.empty())
		return m_rendererGroups["Default"];

	if (m_rendererGroups.count(name) == 0)
	{
		PrimitiveRendererGroup* pRendererGroup = new PrimitiveRendererGroup(m_pParent);
		m_rendererGroups[name] = pRendererGroup;
	}

	return m_rendererGroups[name];
}

void PrimitiveRenderer::AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1, const string& group)
{
	auto pRendererGroup = GetOrCreateRenderGroup(group);
	pRendererGroup->AddLine(v0, v1, color0, color1);
}

void PrimitiveRenderer::AddTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec4& color, bool isWireFrame, const string& group)
{
	auto pRendererGroup = GetOrCreateRenderGroup(group);
	pRendererGroup->AddTriangle(v0, v1, v2, color, isWireFrame);
}

void PrimitiveRenderer::AddRectangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec4& color, bool isWireFrame, const string& group)
{
	auto pRendererGroup = GetOrCreateRenderGroup(group);
	pRendererGroup->AddRectangle(v0, v1, v2, v3, color, isWireFrame);
}

void PrimitiveRenderer::AddSphere(const Vec3& position, float radius, const Vec4& color, bool isWireFrame, const string& group)
{
	auto pRendererGroup = GetOrCreateRenderGroup(group);
	pRendererGroup->AddSphere(position, radius, color, isWireFrame);
}

void PrimitiveRenderer::AddBox(const Vec3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame, const string& group)
{
	auto pRendererGroup = GetOrCreateRenderGroup(group);
	pRendererGroup->AddBox(position, lengthX, lengthY, lengthZ, color, isWireFrame);
}

void PrimitiveRenderer::ClearAll()
{
	for (auto& kvp : m_rendererGroups)
	{
		kvp.second->Clear();
	}
}

void PrimitiveRenderer::Clear(const string& group)
{
	if (group.empty())
		ClearAll();
	else
	{
		auto pRendererGroup = GetOrCreateRenderGroup(group);
		pRendererGroup->Clear();
	}
}

void PrimitiveRenderer::SetVisible(bool bVisible, const string& group)
{
	if (group.empty())
	{
		for (auto& kvp : m_rendererGroups)
		{
			kvp.second->SetVisible(bVisible);
		}
	}
	else
	{
		auto pRendererGroup = GetOrCreateRenderGroup(group);
		pRendererGroup->SetVisible(bVisible);
	}
}

void PrimitiveRenderer::ToggleVisible(const string& group)
{
	if (group.empty())
	{
		for (auto& kvp : m_rendererGroups)
		{
			kvp.second->ToggleVisible();
		}
	}
	else
	{
		auto pRendererGroup = GetOrCreateRenderGroup(group);
		pRendererGroup->ToggleVisible();
	}
}





PrimitiveRendererGroup::PrimitiveRendererGroup(Group* pParent)
	: m_pParent(pParent)
{
	m_pSwitch = new Switch;
	m_pParent->addChild(m_pSwitch);

	m_pGeodeBoxes = new Geode;
	m_pSwitch->addChild(m_pGeodeBoxes);
	m_pGeodeShperes = new Geode;
	m_pSwitch->addChild(m_pGeodeShperes);
	m_pGeodeTriangles = new Geode;
	m_pSwitch->addChild(m_pGeodeTriangles);
	m_pGeodeRectangles = new Geode;
	m_pSwitch->addChild(m_pGeodeRectangles);

	m_pGeodeBoxes->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
	m_pGeodeShperes->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
	m_pGeodeTriangles->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
	m_pGeodeRectangles->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

	m_pLines = new PrimitiveRendererElementLines(pParent);
}

PrimitiveRendererGroup::~PrimitiveRendererGroup()
{
	if (m_pLines != nullptr)
	{
		delete m_pLines;
		m_pLines = nullptr;
	}
}

void PrimitiveRendererGroup::AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1)
{
	m_pLines->AddLine(v0, v1, color0, color1);
}

void PrimitiveRendererGroup::AddTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec4& color, bool isWireFrame)
{
	if (!isWireFrame)
	{
		auto dir0 = v1 - v0;
		dir0.normalize();
		auto dir1 = v2 - v1;
		dir1.normalize();
		auto n = dir0 ^ dir1;
		n.normalize();

		ref_ptr<Vec3Array> pVertexArray = new Vec3Array;
		ref_ptr<DrawElementsUInt> pIndices = new DrawElementsUInt(GL_TRIANGLES, 3);
		ref_ptr<Vec3Array> pNormalArray = new Vec3Array;

		pVertexArray->push_back(v0);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v2);

		pIndices->push_back(0);
		pIndices->push_back(1);
		pIndices->push_back(2);

		pNormalArray->push_back(n);
		pNormalArray->push_back(n);
		pNormalArray->push_back(n);

		ref_ptr<Geometry> pGeometry = new Geometry();
		pGeometry->setVertexArray(pVertexArray);
		pGeometry->addPrimitiveSet(pIndices);

		pGeometry->setNormalArray(pNormalArray);
		pGeometry->setNormalBinding(Geometry::BIND_PER_VERTEX);

		pGeometry->setUseDisplayList(false);
		pGeometry->setUseVertexBufferObjects(true);
		pGeometry->setDataVariance(Object::DYNAMIC);

		ref_ptr<Vec4Array> pColors = new Vec4Array;
		pColors->push_back(color);
		pColors->push_back(color);
		pColors->push_back(color);
		pGeometry->setColorArray(pColors);
		pGeometry->setColorBinding(Geometry::BIND_PER_VERTEX);

		if (color.w() < 1.0f)
		{
			osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
			pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
			pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		m_pGeodeTriangles->addDrawable(pGeometry);
	}
	else
	{
		ref_ptr<Vec3Array> pVertexArray = new Vec3Array;
		pVertexArray->push_back(v0);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v2);
		pVertexArray->push_back(v2);
		pVertexArray->push_back(v0);

		ref_ptr<Geometry> pGeometry = new Geometry();
		pGeometry->setVertexArray(pVertexArray.get());
		pGeometry->addPrimitiveSet(new DrawArrays(GL_LINES, 0, pVertexArray->size()));

		ref_ptr<Vec4Array> pColors = new Vec4Array;
		pColors->push_back(color);
		pGeometry->setColorArray(pColors);
		pGeometry->setColorBinding(Geometry::BIND_OVERALL);

		osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
		pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		if (color.w() < 1.0f)
		{
			pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
			pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}
		m_pGeodeTriangles->addDrawable(pGeometry);
	}
}

void PrimitiveRendererGroup::AddRectangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec4& color, bool isWireFrame)
{
	if (!isWireFrame)
	{
		auto dir0 = v1 - v0;
		dir0.normalize();
		auto dir1 = v2 - v1;
		dir1.normalize();
		auto n = dir0 ^ dir1;
		n.normalize();

		ref_ptr<Vec3Array> pVertexArray = new Vec3Array;
		ref_ptr<DrawElementsUInt> pIndices = new DrawElementsUInt(GL_TRIANGLES, 6);
		ref_ptr<Vec3Array> pNormalArray = new Vec3Array;

		pVertexArray->push_back(v0);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v2);
		pVertexArray->push_back(v3);

		pIndices->push_back(0);
		pIndices->push_back(1);
		pIndices->push_back(2);
		pIndices->push_back(2);
		pIndices->push_back(1);
		pIndices->push_back(3);

		pNormalArray->push_back(n);
		pNormalArray->push_back(n);
		pNormalArray->push_back(n);
		pNormalArray->push_back(n);

		ref_ptr<Geometry> pGeometry = new Geometry();
		pGeometry->setVertexArray(pVertexArray);
		pGeometry->addPrimitiveSet(pIndices);

		pGeometry->setNormalArray(pNormalArray);
		pGeometry->setNormalBinding(Geometry::BIND_PER_VERTEX);

		pGeometry->setUseDisplayList(false);
		pGeometry->setUseVertexBufferObjects(true);
		pGeometry->setDataVariance(Object::DYNAMIC);

		ref_ptr<Vec4Array> pColors = new Vec4Array;
		pColors->push_back(color);
		pColors->push_back(color);
		pColors->push_back(color);
		pColors->push_back(color);
		pGeometry->setColorArray(pColors);
		pGeometry->setColorBinding(Geometry::BIND_PER_VERTEX);

		if (color.w() < 1.0f)
		{
			osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
			pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
			pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		m_pGeodeRectangles->addDrawable(pGeometry);
	}
	else
	{
		ref_ptr<Vec3Array> pVertexArray = new Vec3Array;
		pVertexArray->push_back(v0);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v1);
		pVertexArray->push_back(v2);
		pVertexArray->push_back(v2);
		pVertexArray->push_back(v3);
		pVertexArray->push_back(v3);
		pVertexArray->push_back(v0);

		ref_ptr<Geometry> pGeometry = new Geometry();
		pGeometry->setVertexArray(pVertexArray.get());
		pGeometry->addPrimitiveSet(new DrawArrays(GL_LINES, 0, pVertexArray->size()));

		ref_ptr<Vec4Array> pColors = new Vec4Array;
		pColors->push_back(color);
		pGeometry->setColorArray(pColors);
		pGeometry->setColorBinding(Geometry::BIND_OVERALL);

		osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
		pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		if (color.w() < 1.0f)
		{
			pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
			pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}
		m_pGeodeRectangles->addDrawable(pGeometry);
	}
}

void PrimitiveRendererGroup::AddSphere(const Vec3& position, float radius, const Vec4& color, bool isWireFrame)
{
	ref_ptr<ShapeDrawable> pGeometry = new ShapeDrawable(new Sphere(position, radius));
	pGeometry->setColor(color);
	osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
	pGeometry->setStateSet(pSS);

	pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	if (color.w() < 1.0f)
	{
		pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
		pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
	if (isWireFrame)
	{
		ref_ptr<PolygonMode> pMode = new PolygonMode;
		pMode->setMode(PolygonMode::FRONT_AND_BACK, PolygonMode::Mode::LINE);
		pSS->setAttributeAndModes(pMode, StateAttribute::ON);
	}

	m_pGeodeShperes->addDrawable(pGeometry);
}

void PrimitiveRendererGroup::AddBox(const Vec3& position, float lengthX, float lengthY, float lengthZ, const Vec4& color, bool isWireFrame)
{
	ref_ptr<ShapeDrawable> pGeometry = new ShapeDrawable(new Box(position, lengthX, lengthY, lengthZ));
	pGeometry->setColor(color);
	osg::StateSet * pSS = pGeometry->getOrCreateStateSet();
	pGeometry->setStateSet(pSS);

	pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	if (color.w() < 1.0f)
	{
		pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
		pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
	if (isWireFrame)
	{
		ref_ptr<PolygonMode> pMode = new PolygonMode;
		pMode->setMode(PolygonMode::FRONT_AND_BACK, PolygonMode::Mode::LINE);
		pSS->setAttributeAndModes(pMode, StateAttribute::ON);
	}

	m_pGeodeBoxes->addDrawable(pGeometry);
}

void PrimitiveRendererGroup::Clear()
{
	m_pLines->Clear();

	m_pGeodeBoxes->removeDrawables(0, m_pGeodeBoxes->getNumDrawables());
	m_pGeodeShperes->removeDrawables(0, m_pGeodeShperes->getNumDrawables());
	m_pGeodeTriangles->removeDrawables(0, m_pGeodeTriangles->getNumDrawables());
	m_pGeodeRectangles->removeDrawables(0, m_pGeodeRectangles->getNumDrawables());
}

void PrimitiveRendererGroup::SetVisible(bool bVisible)
{
	if (m_pSwitch)
	{
		m_pSwitch->setValue(0, bVisible);
	}
}

void PrimitiveRendererGroup::ToggleVisible()
{
	if (m_pSwitch)
	{
		m_pSwitch->setValue(0, !m_pSwitch->getValue(0));
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveRendererElements::PrimitiveRendererElements(Group* pParent)
	: m_pParent(pParent), m_pGeode(nullptr), m_pGeometry(nullptr), m_pVertexArray(nullptr), m_pIndices(nullptr), m_pNormalArray(nullptr), m_pColorArray(nullptr)
{
	m_pGeode = new Geode;
	m_pParent->addChild(m_pGeode);
	m_pGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

	m_pGeometry = new Geometry();
	m_pGeode->addDrawable(m_pGeometry);

	m_pGeometry->setUseDisplayList(false);
	m_pGeometry->setUseVertexBufferObjects(true);
	m_pGeometry->setDataVariance(Object::DYNAMIC);

	osg::StateSet * pSS = m_pGeometry->getOrCreateStateSet();
	pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pSS->setMode(GL_BLEND, osg::StateAttribute::ON);
	pSS->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	// Line Width ==>>
	ref_ptr<LineWidth> linewidth = new LineWidth();
	linewidth->setWidth(5.0f);
	pSS->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
	// <<== Line Width
}

PrimitiveRendererElements::~PrimitiveRendererElements()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveRendererElementLines::PrimitiveRendererElementLines(Group* pParent)
	: PrimitiveRendererElements(pParent)
{
}

PrimitiveRendererElementLines::~PrimitiveRendererElementLines()
{

}

void PrimitiveRendererElementLines::AddLine(const Vec3& v0, const Vec3& v1, const Vec4& color0, const Vec4& color1)
{
	if (m_pVertexArray == nullptr)
	{
		m_pVertexArray = new Vec3Array;
		m_pGeometry->setVertexArray(m_pVertexArray);
	}
	if (m_pIndices == nullptr)
	{
		m_pIndices = new DrawElementsUInt(GL_LINES);
		m_pGeometry->addPrimitiveSet(m_pIndices);
	}
	if (m_pColorArray == nullptr)
	{
		m_pColorArray = new Vec4Array;
		m_pGeometry->setColorArray(m_pColorArray);
		m_pGeometry->setColorBinding(Geometry::BIND_PER_VERTEX);
	}

	m_pVertexArray->push_back(v0);
	m_pIndices->push_back(m_pVertexArray->size() - 1);
	m_pColorArray->push_back(color0);

	m_pVertexArray->push_back(v1);
	m_pIndices->push_back(m_pVertexArray->size() - 1);
	m_pColorArray->push_back(color1);

	m_pVertexArray->dirty();
	m_pIndices->dirty();
	m_pColorArray->dirty();

	m_pGeometry->dirtyBound();
	m_pGeode->dirtyBound();
}

void PrimitiveRendererElementLines::Clear()
{
	if (m_pVertexArray != nullptr) m_pVertexArray->clear();
	if (m_pIndices != nullptr) m_pIndices->clear();
	if (m_pColorArray != nullptr) m_pColorArray->clear();

	if (m_pVertexArray != nullptr) m_pVertexArray->dirty();
	if (m_pIndices != nullptr) m_pIndices->dirty();
	if (m_pColorArray != nullptr) m_pColorArray->dirty();
}