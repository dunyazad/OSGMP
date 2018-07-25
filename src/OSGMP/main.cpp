//#include <chrono>
//using namespace std;

#include "OSGMP.h"
using namespace VEFM;

OSGMP<Vec3>* g_pMP = nullptr;

PrimitiveRendererVEFM<Vec3>* pvd = nullptr;
string g_pickedObejctName = "";
ref_ptr<Group> g_pRootNode;

StopWatch g_stopWatch;

map<tuple<Mesh<Vec3>*, Face<Vec3>*>, set<Vec3>> g_result;
int g_count = 0;

set<Edge<Vec3>*> g_visitedEdges;

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

	KeyboardEventHandler() {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN) :
		{
			switch (ea.getKey())
			{
			case 'q':
			{
				printf("Q\n");

				pvd->Clear();

				auto pMesh = g_pMP->GetOrCreateMesh("TEMP");

				pMesh->GetOrCreateFace(Vec3(0, 0, 0), Vec3(10, 0, 0), Vec3(5, 0, 5));
				pMesh->GetOrCreateFace(Vec3(10, 0, 0), Vec3(15, 0, 5), Vec3(5, 0, 5));
				pMesh->GetOrCreateFace(Vec3(10, 0, 0), Vec3(20, 0, 0), Vec3(15, 0, 5));
				pMesh->GetOrCreateFace(Vec3(20, 0, 0), Vec3(25, 0, 5), Vec3(15, 0, 5));
				pMesh->GetOrCreateFace(Vec3(20, 0, 0), Vec3(30, 0, 0), Vec3(25, 0, 5));

				pMesh->GetOrCreateFace(Vec3(0, 0, 0), Vec3(5, 0, -5), Vec3(10, 0, 0));
				pMesh->GetOrCreateFace(Vec3(10, 0, 0), Vec3(5, 0, -5), Vec3(15, 0, -5));
				pMesh->GetOrCreateFace(Vec3(10, 0, 0), Vec3(15, 0, -5), Vec3(20, 0, 0));
				pMesh->GetOrCreateFace(Vec3(20, 0, 0), Vec3(15, 0, -5), Vec3(25, 0, -5));
				pMesh->GetOrCreateFace(Vec3(20, 0, 0), Vec3(25, 0, -5), Vec3(30, 0, 0));

				g_pMP->UpdateModel();

				pMesh->PrintInformation();

				return true;
			}
			case 'w':
			{
				printf("W\n");

				pvd->Clear();

				auto pMesh = g_pMP->GetOrCreateMesh("TEMP");

				auto pF0 = pMesh->GetFaces()[0];
				auto pF1 = pMesh->GetFaces()[1];

				pMesh->FlipEdge(pF0, pF1);

				g_pMP->UpdateModel();

				pMesh->PrintInformation();

				return true;
			}
			case 'l':
				printf("L\n");
				{
					auto pMesh = g_pMP->GetMesh("TEMP");
					auto pE = pMesh->GetEdge(Vec3(10, 0, 0), Vec3(20, 0, 0));

					pMesh->PrintInformation();

					//pMesh->CollapseEdge(pE);
					//pMesh->SplitEdge(pE);

					auto pF = pMesh->GetFace(Vec3(0, 0, 0), Vec3(10, 0, 0), Vec3(5, 0, 5));
					pMesh->SplitFace(pF, Vec3(5, 0, 2.5));

					g_pMP->UpdateModel();

					pMesh->PrintInformation();
				}
				return true;
			case 'f':
				printf("F\n");
				{
					printf("================================================================================\n");

					auto pMesh = g_pMP->GetMesh("Mx");
					//auto pE = pMesh->FindFirstMetBorderEdge();
					//if (pE != nullptr)
					//{
					//	pvd->AddLineByEdge(pE, V4_RED, V4_RED);
					//}
					//auto&& edges = pMesh->FindBorderEdges();
					auto&& edges = pMesh->FindNonManifoldEdges();
					for (auto& pE : edges)
					{
						if (g_visitedEdges.count(pE) == 0)
						{
							g_visitedEdges.insert(pE);

							pvd->AddLineByEdge(pE, V4_GREEN, V4_GREEN);
							for (auto& pF : pE->GetInsidentFaces())
							{
								pvd->AddTriangleByFace(pF, V4_RED, true);
								pvd->AddBox(pF->V0()->P(), 0.01f, 0.01f, 0.01f, V4_RED, true);
								pvd->AddBox(pF->V1()->P(), 0.008f, 0.008f, 0.008f, V4_GREEN, true);
								pvd->AddBox(pF->V2()->P(), 0.004f, 0.004f, 0.004f, V4_BLUE, true);

								printf("%f, %f, %f\n", pF->V0()->P().x(), pF->V0()->P().y(), pF->V0()->P().z());
								printf("%f, %f, %f\n", pF->V1()->P().x(), pF->V1()->P().y(), pF->V1()->P().z());
								printf("%f, %f, %f\n", pF->V2()->P().x(), pF->V2()->P().y(), pF->V2()->P().z());
							}
							break;
						}
					}
				}
				return true;
			case 'm':
				printf("M\n");
				g_pMP->TogglePolygonMode();
				return true;
			case '1':
				g_pMP->ToggleVisible("Mx");
				return true;
			case '2':
				g_pMP->ToggleVisible();
				return true;
			case 'z':
				printf("Z\n");
				{
					//map<tuple<Mesh<Vec3>*, Face<Vec3>*>, set<pair<Vec3, Vec3>>> result;
					
					g_stopWatch.Start();
					
					g_pMP->CheckIntersection("Mx", "MxTeeth", g_result);
					
					auto seconds = g_stopWatch.Stop().first;
					printf("seconds %f\n", seconds);

					//for (auto& kvp : result)
					//{
					//	for (auto& vv : kvp.second)
					//	{
					//		pvd->AddLine(vv.first, vv.second, V4_RED, V4_RED);
					//	}
					//}

					g_pMP->SplitFaces(g_result);

					g_pMP->UpdateModel();
				}
				return false;
			case 'x':
				printf("X\n");
				{
				}
				return false;

			case 'c':
				printf("C\n");
				pvd->Clear();
				return false;
			case 'b':
				printf("B\n");
				{
					auto pMesh = g_pMP->GetMesh("Mx");
					auto& edges = pMesh->GetEdges();
					
					for (auto& pE : edges)
					{
						if (pE->GetFlag("IntersectionBorder"))
						{
							pvd->AddLineByEdge(pE, V4_RED, V4_RED);
						}
					}
				}
				return false;
			case 's':
				printf("S\n");
				{
					g_pMP->UpdateModel();

					//g_pMP->SaveSTLFile("Mx", "../../res/R1.stl", true);
					//g_pMP->SaveSTLFile("MxTeeth", "../../res/R2.stl", true);

					g_pMP->SaveSTLFile("TEMP", "../../res/TEMP.stl", true);
				}
				return false;
			case 'a':
				printf("A\n");
				{
					auto pMesh = g_pMP->GetMesh("Mx");

					for (auto& pF : pMesh->GetFaces())
					{
						auto& p0 = pF->V0()->GetPosition();
						auto& p1 = pF->V1()->GetPosition();
						auto& p2 = pF->V2()->GetPosition();

						auto d10 = p1 - p2;
						auto d20 = p2 - p2;
						auto normal = d10 ^ d20;
						normal.normalize();

						p0 -= (normal);
						p1 -= (normal);
						p2 -= (normal);
					}

					g_pMP->UpdateModel("Mx");
				}
				return false;
			default:
				break;

			}
		}
		}
		//return false to allow mouse manipulation
		return false;
	}
};

class MouseEventHandler : public osgGA::GUIEventHandler
{
public:

	MouseEventHandler() {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::PUSH) :
		{
			if (ea.getButtonMask() | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				printf("Clicked...!!!\n");
			}
			return true;
		}
			break;
		default:
			break;
		}
		return false;
	}
};

class PickHandler : public osgGA::GUIEventHandler
{
public:
	virtual void PickingResult(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osgUtil::LineSegmentIntersector::Intersection& result)
	{
		osg::Node* node = 0;
		osg::Group* parent = 0;

		osg::NodePath& nodePath = result.nodePath;
		node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
		parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
		if (parent && node)
		{
			//printf("================================================================\n");
			//for (auto& i : result.indexList)
			//{
			//	printf("picked index %d\n", i);
			//}
			//printf("================================================================\n");

			auto name = result.drawable->asGeometry()->getName();

			auto va = (Vec3Array*)result.drawable->asGeometry()->getVertexArray();
			auto& v0 = va->at(result.indexList.at(0));
			auto& v1 = va->at(result.indexList.at(1));
			auto& v2 = va->at(result.indexList.at(2));

			printf("Picked Obejct Name %s\n", name.c_str());

			if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL)
			{
			}
			else if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SHIFT)
			{
			}
			else if (ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT)
			{
			}
			else
			{
				//auto pMesh = g_pMP->GetMesh(name);

				//float d0 = (result.getWorldIntersectPoint() - v0).length2();
				//float d1 = (result.getWorldIntersectPoint() - v1).length2();
				//float d2 = (result.getWorldIntersectPoint() - v2).length2();

				////Vec3 p;
				////if (d0 < d1 && d0 < d2) p = v0;
				////if (d1 < d0 && d1 < d2) p = v1;
				////if (d2 < d0 && d2 < d1) p = v2;

				////auto pV = pMesh->GetVertex(Vec3toV3(p));
				////if (pV)
				////{
				////	//pV->QueryDelete();
				////	//g_pMP->UpdateModel(name);

				////	set<Face*> faces;
				////	auto& ies = pV->GetIncidentEdges();
				////	for (auto& pE : ies)
				////	{
				////		auto& ifs = pE->GetInsidentFaces();
				////		for (auto& pF : ifs)
				////		{
				////			faces.insert(pF);
				////		}
				////	}

				////	for (auto& pF : faces)
				////	{
				////		//pvd->AddTriangleByFace(pF, V4_RED, false);
				////		pF->QueryDelete();
				////	}

				////	g_pMP->UpdateModel(name);
				////}

				//auto pFace = pMesh->GetFace(v0, v1, v2);
				//if (pFace)
				//{
				//	//pvd->AddTriangleByFace(pFace, V4_RED, false);

				//	pFace->QueryDelete();
				//	g_pMP->UpdateModel(name);
				//}

				////////pvd->AddTriangle(v0, v1, v2, V4_GREEN, false);
			}
		}
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		//if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE || ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON || !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL))
		//	return false;

		if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
			if (viewer)
			{
				osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
				osgUtil::IntersectionVisitor iv(intersector.get());
				viewer->getCamera()->accept(iv);
				if (intersector->containsIntersections())
				{
					auto result = *(intersector->getIntersections().begin());
					PickingResult(ea, aa, result);
				}
				else
				{
					g_pickedObejctName = "";
					printf("g_pickedObejctName %s\n", g_pickedObejctName.c_str());
				}
			}
			return false;
		}
		return false;
	}
};

void DrawNode(const OctreeNode<Vertex<Vec3>, Vec3>* pNode)
{
	if (pNode->ChildExists())
	{
		auto& min = pNode->GetMin();
		auto& max = pNode->GetMax();

		if (pNode->GetLevel() == 0)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(1, 1, 1, 1), true, "LEVEL0");
		}
		else if (pNode->GetLevel() == 1)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0, 1, 1, 1), true, "LEVEL1");
		}
		else if (pNode->GetLevel() == 2)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(1, 0, 1, 1), true, "LEVEL2");
		}
		else if (pNode->GetLevel() == 3)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(1, 1, 0, 1), true, "LEVEL3");
		}
		else if (pNode->GetLevel() == 4)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(1, 0, 0, 1), true, "LEVEL4");
		}
		else if (pNode->GetLevel() == 5)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0, 1, 0, 1), true, "LEVEL5");
		}
		else if (pNode->GetLevel() == 6)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0, 0, 1, 1), true, "LEVEL6");
		}
		else if (pNode->GetLevel() == 7)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0.5f, 1, 1, 1), true, "LEVEL7");
		}
		else if (pNode->GetLevel() == 8)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0.5f, 0.5f, 1, 1), true, "LEVEL8");
		}
		else if (pNode->GetLevel() == 9)
		{
			pvd->AddBox(pNode->GetCenter(), max.x() - min.x(), max.y() - min.y(), max.z() - min.z(), Vec4(0.5f, 0.5f, 0.5f, 1), true, "LEVEL9");
		}

		auto children = pNode->GetChildren();
		for (int i = 0; i < OctreeNodeIndex::INDEX_SIZE; i++)
		{
			if (children[i] != nullptr)
			{
				DrawNode(children[i]);
			}
		}
	}
}

void DrawVolumeInfo(const Octree<Vertex<Vec3>, Vec3>* pVolumeInfo)
{
	auto pRootNode = pVolumeInfo->GetRootNode();
	DrawNode(pRootNode);
}
//
//void Triangluate(const Vec3& fv0, const Vec3& fv1, const Vec3& fv2, const vector<Vec3>& points, vector<tuple<Vec3, Vec3, Vec3>>& result)
//{
//	auto d01 = fv1 - fv0;
//	auto d02 = fv2 - fv0;
//	auto normal = d02 ^ d01;
//	normal.normalize();
//	auto center = (fv0 + fv1 + fv2) / 3;
//
//	Matrix m;
//	m.makeRotate(Vec3(0, 0, 1), normal);
//	m.translate(center);
//
//	auto im = Matrix::inverse(m);
//
//	ref_ptr<Vec3Array> pVertices = new Vec3Array;
//
//	auto pfv0 = im * fv0;
//	pVertices->push_back(Vec3(pfv0.x(), pfv0.y(), 0));
//	auto pfv1 = im * fv1;
//	pVertices->push_back(Vec3(pfv1.x(), pfv1.y(), 0));
//	auto pfv2 = im * fv2;
//	pVertices->push_back(Vec3(pfv2.x(), pfv2.y(), 0));
//
//	for (auto& p : points)
//	{
//		auto pp = im * p;
//		pVertices->push_back(Vec3(pp.x(), pp.y(), 0));
//	}
//
//	ref_ptr<osgUtil::DelaunayTriangulator> pDT = new osgUtil::DelaunayTriangulator;
//	pDT->setInputPointArray(pVertices);
//	pDT->triangulate();
//
//	auto pTriangles = pDT->getTriangles();
//	for (unsigned int i = 0; i < pTriangles->size() / 3; i++)
//	{
//		auto i0 = pTriangles->at(i * 3 + 0);
//		auto i1 = pTriangles->at(i * 3 + 1);
//		auto i2 = pTriangles->at(i * 3 + 2);
//		auto& v0 = pVertices->at(i0);
//		auto& v1 = pVertices->at(i1);
//		auto& v2 = pVertices->at(i2);
//
//		result.push_back(make_tuple(m * v0, m * v1, m * v2));
//	}
//}

namespace VEFM
{

	template<typename T> class TriangleTreeNode;
	template<typename T> class TriangleTree;

	template<typename T>
	class TriangleTreeNode : public BinaryTreeNode<T>
	{
	public:
		void Populate(const vector<int>& indices, int from, int to)
		{
			if (to - from > 2)
			{
				BinaryTreeNode<int>* pLeftChild = m_pTree->CreateNode(this, true);
				((TriangleTreeNode<T>*)pLeftChild)->Populate(indices, from, from + (int)((to - from) * 0.5f));
				BinaryTreeNode<int>* pRightChild = m_pTree->CreateNode(this, false);
				((TriangleTreeNode<T>*)pRightChild)->Populate(indices, from + (int)((to - from) * 0.5f) + 1, to);
			}
			else
			{
				for (int i = from; i <= to; i++)
				{
					m_elements.push_back(i);
				}
			}
		}

		void MergeChild(const vector<Vec3> vertices, const vector<int>& indices)
		{
			if (m_pLeftChild != nullptr)
			{
				((TriangleTreeNode<T>*)m_pLeftChild)->MergeChild(vertices, indices);
			
				if (m_pLeftChild->IsLeaf())
				{
					for (auto& v : m_pLeftChild->GetElements())
					{
						m_elements.push_back(v);
					}

					RemoveLeftChild();
				}
			}

			if (m_pRightChild != nullptr)
			{
				((TriangleTreeNode<T>*)m_pRightChild)->MergeChild(vertices, indices);

				if (m_pRightChild->IsLeaf())
				{
					for (auto& v : m_pRightChild->GetElements())
					{
						m_elements.push_back(v);
					}

					RemoveRightChild();
				}
			}
		}

	protected:
		vector<tuple<int, int, int>> triangleIndices;

		TriangleTreeNode(BinaryTree<T>* pTree, BinaryTreeNode<T>* pParent, const string& name)
			: BinaryTreeNode<T>(pTree, pParent, name)
		{

		}

	public:
		friend class TriangleTree<T>;
	};

	template<typename T>
	class TriangleTree : public BinaryTree<T>
	{
	public:
		TriangleTree()
			: BinaryTree()
		{
			if (m_pRootNode != nullptr)
			{
				m_pRootNode = new TriangleTreeNode<T>(this, nullptr, "0");
			}
		}

		void Populate(const vector<int>& indices, int from, int to)
		{
			((TriangleTreeNode<T>*)m_pRootNode)->Populate(indices, from, to);
		}

		void MergeChildNodes(const vector<Vec3> vertices, const vector<int>& indices)
		{
			((TriangleTreeNode<T>*)m_pRootNode)->MergeChild(vertices, indices);
		}
	};
}

void PrintNode(TreeNode<int>* pNode)
{
	if (pNode->GetChildren().size() > 0)
	{
		for (auto& pC : pNode->GetChildren())
		{
			PrintNode(pC);
		}
	}
	//else
	{
		if (pNode->GetElements().size() > 0)
		{
			printf("Node Name : %s\n", pNode->GetName().c_str());
			for (auto& i : pNode->GetElements())
			{
				printf("%d\n", i);
			}
		}
	}
}

void PrintNode(BinaryTreeNode<int>* pNode)
{
	auto pLeftChild = pNode->GetLeftChild();
	if (pLeftChild != nullptr)
	{
		PrintNode(pLeftChild);
	}

	auto pRightChild = pNode->GetRightChild();
	if (pRightChild != nullptr)
	{
		PrintNode(pRightChild);
	}
	//else
	{
		if (pNode->GetElements().size() > 0)
		{
			printf("Node Name : %s\n", pNode->GetName().c_str());
			for (auto& i : pNode->GetElements())
			{
				printf("%d\n", i);
			}
		}
	}
}

int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewOnSingleScreen(0);
	viewer.getCamera()->setClearColor(Vec4(0.3f, 0.5f, 0.7f, 1.0f));
	//viewer.setUpViewInWindow(300, 300, 800, 600, 1);
	viewer.setUpViewInWindow(300, 300, 800, 600, 0);

	g_pRootNode = new Group;

	viewer.addEventHandler(new KeyboardEventHandler);
	viewer.addEventHandler(new PickHandler);

	// run optimization over the scene graph
	osgUtil::Optimizer optimzer;
	optimzer.optimize(g_pRootNode);

	PrimitiveRendererVEFM<Vec3> vd(g_pRootNode);
	pvd = &vd;

	// add a viewport to the viewer and attach the scene graph.
	viewer.setSceneData(g_pRootNode);

	OSGMP<Vec3> mp(g_pRootNode);
	g_pMP = &mp;
	g_pMP->SetVD(pvd);

	
	
#pragma region Model Intersection
	//mp.LoadABDFile("Mx", "../../res/MxBone.abd");
	//mp.LoadABDFile("MxTeeth", "../../res/MxTeeth.abd");
#pragma endregion



	Vec3 fv0(0, 0, -400);
	Vec3 fv1(250, 0, 0);
	Vec3 fv2(500, 0, -400);
	pvd->AddTriangle(fv0, fv1, fv2, Vec4(GREEN, 0.3), true);

	vector<Vec3> vertices;
	vertices.push_back(fv0);
	vertices.push_back(fv1);
	vertices.push_back(fv2);
	vertices.push_back(Vec3(110, 0, -260));
	vertices.push_back(Vec3(120, 0, -330));
	vertices.push_back(Vec3(190, 0, -120));
	vertices.push_back(Vec3(230, 0, -180));
	vertices.push_back(Vec3(170, 0, -230));
	vertices.push_back(Vec3(200, 0, -250));
	vertices.push_back(Vec3(150, 0, -310));
	vertices.push_back(Vec3(230, 0, -300));
	vertices.push_back(Vec3(230, 0, -210));
	vertices.push_back(Vec3(270, 0, -190));
	vertices.push_back(Vec3(240, 0, -130));
	vertices.push_back(Vec3(230, 0, -70));
	vertices.push_back(Vec3(280, 0, -100));
	vertices.push_back(Vec3(310, 0, -210));
	vertices.push_back(Vec3(240, 0, -250));
	vertices.push_back(Vec3(270, 0, -270));
	vertices.push_back(Vec3(290, 0, -230));
	vertices.push_back(Vec3(330, 0, -240));
	vertices.push_back(Vec3(340, 0, -270));
	vertices.push_back(Vec3(320, 0, -320));
	vertices.push_back(Vec3(130, 0, -360));
	vertices.push_back(Vec3(390, 0, -380));
	vertices.push_back(Vec3(270, 0, -350));
	vertices.push_back(Vec3(380, 0, -330));
	vertices.push_back(Vec3(360, 0, -220));

	sort(vertices.begin(), vertices.end());

	vector<int> indices;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	

	TriangleTree<int> tree;
	tree.Populate(indices, 0, indices.size() - 1);

	tree.MergeChildNodes(vertices, indices);

	PrintNode(tree.GetRootNode());


#pragma region Projection Test
	//mp.LoadABDFile("Mx", "../../res/random plane.abd");
	//auto pMesh = mp.GetMesh("Mx");

	//set<Vertex<Vec3>*> vertices;
	//pMesh->GetVertices(vertices);

	//vector<Vec3> positions;
	//for (auto& pV : vertices)
	//{
	//	g_pMP->ProjectToPlane(Vec3(0, 0, 10), Vec3(0, -1, -1), pV->P(), pV->P());
	//}
#pragma endregion


	

	//auto pMD = mp.GetOrCreateMesh("Md");

	//Vec3 normal(0, -1, -1);
	//Vec3 position(0, 0, 10);

	//for (auto& pF : pMesh->GetFaces())
	//{
	//	auto& p0 = pF->V0()->P();
	//	auto& p1 = pF->V1()->P();
	//	auto& p2 = pF->V2()->P();
	//	
	//	Vec3 np0;
	//	Vec3 np1;
	//	Vec3 np2;
	//	bool r0 = IntersectRayPlane<Vec3>(p0, -normal, position, normal, np0);
	//	bool r1 = IntersectRayPlane<Vec3>(p1, -normal, position, normal, np1);
	//	bool r2 = IntersectRayPlane<Vec3>(p2, -normal, position, normal, np2);

	//	pMD->GetOrCreateFace(np0, np1, np2);
	//}

	mp.UpdateModel();



	return viewer.run();
}
