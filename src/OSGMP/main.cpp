//#include <chrono>
//using namespace std;

#include "OSGMP.h"
using namespace VEFM;

OSGMP<V3>* g_pMP = nullptr;

PrimitiveRendererVEFM<V3>* pvd = nullptr;
string g_pickedObejctName = "";
ref_ptr<Group> g_pRootNode;

StopWatch g_stopWatch;

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
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL0");
				return true;
			}
			case 'w':
				printf("W\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL1");
				return true;
			case 'e':
				printf("E\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL2");
				return true;
			case 'r':
				printf("R\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL3");
				return true;
			case 't':
				printf("T\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL4");
				return true;
			case 'y':
				printf("Y\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL5");
				return true;
			case 'u':
				printf("U\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL6");
				return true;
			case 'i':
				printf("Y\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL7");
				return true;
			case 'o':
				printf("O\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL8");
				return true;
			case 'p':
				printf("Y\n");
				pvd->SetVisible(false);
				pvd->SetVisible(true, "LEVEL9");
				return true;
			case '`':
				printf("`\n");
				pvd->ToggleVisible();
				return true;
			case 'f':
				printf("F\n");
				{
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
						pvd->AddLineByEdge(pE, V4_RED, V4_RED);
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
					map<tuple<Mesh<V3>*, Face<V3>*>, set<pair<V3, V3>>> result;
					
					g_stopWatch.Start();
					
					g_pMP->CheckIntersection("Mx", "MxTeeth", result);
					
					auto seconds = g_stopWatch.Stop().first;
					printf("seconds %f\n", seconds);

					//for (auto& kvp : result)
					//{
					//	for (auto& vv : kvp.second)
					//	{
					//		pvd->AddLine(V3toVec3(vv.first), V3toVec3(vv.second), V4_RED, V4_RED);
					//	}
					//}

					g_pMP->SplitFaces(result);

					g_pMP->UpdateModel();
				}
				return false;
			case 'x':
				printf("X\n");
				{
					vector<string> names;
					names.push_back("T11"); names.push_back("T12"); names.push_back("T13"); names.push_back("T14"); names.push_back("T15"); names.push_back("T16"); names.push_back("T17");
					names.push_back("T21"); names.push_back("T22"); names.push_back("T23"); names.push_back("T24"); names.push_back("T25"); names.push_back("T26"); names.push_back("T27");

					map<tuple<Mesh<V3>*, Face<V3>*>, set<pair<V3, V3>>> result;

					g_stopWatch.Start();

					g_pMP->CheckIntersections("Mx", names, result);

					auto seconds = g_stopWatch.Stop().first;
					printf("seconds %f\n", seconds);

					for (auto& kvp : result)
					{
						for (auto& vv : kvp.second)
						{
							//pvd->AddLine(V3toVec3(vv.first), V3toVec3(vv.second), V4_RED, V4_RED);
						}
					}

					g_pMP->SplitFaces(result);

					g_pMP->UpdateModel();
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

					g_pMP->SaveSTLFile("Mx", "../../res/R1.stl", true);
					g_pMP->SaveSTLFile("MxTeeth", "../../res/R2.stl", true);
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
				auto pMesh = g_pMP->GetMesh(name);

				float d0 = (result.getWorldIntersectPoint() - v0).length2();
				float d1 = (result.getWorldIntersectPoint() - v1).length2();
				float d2 = (result.getWorldIntersectPoint() - v2).length2();

				//Vec3 p;
				//if (d0 < d1 && d0 < d2) p = v0;
				//if (d1 < d0 && d1 < d2) p = v1;
				//if (d2 < d0 && d2 < d1) p = v2;

				//auto pV = pMesh->GetVertex(Vec3toV3(p));
				//if (pV)
				//{
				//	//pV->QueryDelete();
				//	//g_pMP->UpdateModel(name);

				//	set<Face*> faces;
				//	auto& ies = pV->GetIncidentEdges();
				//	for (auto& pE : ies)
				//	{
				//		auto& ifs = pE->GetInsidentFaces();
				//		for (auto& pF : ifs)
				//		{
				//			faces.insert(pF);
				//		}
				//	}

				//	for (auto& pF : faces)
				//	{
				//		//pvd->AddTriangleByFace(pF, V4_RED, false);
				//		pF->QueryDelete();
				//	}

				//	g_pMP->UpdateModel(name);
				//}

				auto pFace = pMesh->GetFace(Vec3toV3(v0), Vec3toV3(v1), Vec3toV3(v2));
				if (pFace)
				{
					//pvd->AddTriangleByFace(pFace, V4_RED, false);

					pFace->QueryDelete();
					g_pMP->UpdateModel(name);
				}

				//////pvd->AddTriangle(v0, v1, v2, V4_GREEN, false);
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

void DrawNode(const OctreeNode<Vertex<V3>>* pNode)
{
	if (pNode->ChildExists())
	{
		auto& min = pNode->GetMin();
		auto& max = pNode->GetMax();

		if (pNode->GetLevel() == 0)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(1, 1, 1, 1), true, "LEVEL0");
		}
		else if (pNode->GetLevel() == 1)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0, 1, 1, 1), true, "LEVEL1");
		}
		else if (pNode->GetLevel() == 2)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(1, 0, 1, 1), true, "LEVEL2");
		}
		else if (pNode->GetLevel() == 3)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(1, 1, 0, 1), true, "LEVEL3");
		}
		else if (pNode->GetLevel() == 4)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(1, 0, 0, 1), true, "LEVEL4");
		}
		else if (pNode->GetLevel() == 5)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0, 1, 0, 1), true, "LEVEL5");
		}
		else if (pNode->GetLevel() == 6)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0, 0, 1, 1), true, "LEVEL6");
		}
		else if (pNode->GetLevel() == 7)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0.5f, 1, 1, 1), true, "LEVEL7");
		}
		else if (pNode->GetLevel() == 8)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0.5f, 0.5f, 1, 1), true, "LEVEL8");
		}
		else if (pNode->GetLevel() == 9)
		{
			pvd->AddBoxByV3(pNode->GetCenter(), max.x - min.x, max.y - min.y, max.z - min.z, Vec4(0.5f, 0.5f, 0.5f, 1), true, "LEVEL9");
		}


		auto children = pNode->GetChildren();
		for (int i = 0; i < OctreeNode<Vertex<V3>>::INDEX_SIZE; i++)
		{
			if (children[i] != nullptr)
			{
				DrawNode(children[i]);
			}
		}
	}
}

void DrawVolumeInfo(const Octree<Vertex<V3>>* pVolumeInfo)
{
	auto pRootNode = pVolumeInfo->GetRootNode();
	DrawNode(pRootNode);
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

	PrimitiveRendererVEFM<V3> vd(g_pRootNode);
	pvd = &vd;

	// add a viewport to the viewer and attach the scene graph.
	viewer.setSceneData(g_pRootNode);


	OSGMP<V3> mp(g_pRootNode);
	g_pMP = &mp;
	g_pMP->SetVD(pvd);

	//mp.LoadABDFile("Mx", "../../res/MxBone.abd");
	//mp.LoadABDFile("MxTeeth", "../../res/MxTeeth.abd");

	return viewer.run();
}
