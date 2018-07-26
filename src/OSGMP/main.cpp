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
			case 'r':
			{
				printf("R\n");
				auto pMesh = g_pMP->GetMesh("Triangle");
				pMesh->RefineFaces();

				g_pMP->UpdateModel();

				return false;
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
	class TriangleTreeNode
	{
	public:
		inline const string& GetName() const { return m_name; }
		inline TriangleTreeNode<T>* GetParent() { return m_pParent; }
		inline TriangleTreeNode<T>* GetLeftChild() { return m_pLeftChild; }
		inline TriangleTreeNode<T>* GetRightChild() { return m_pRightChild; }
		inline vector<T>& GetElements() { return m_elements; }

		inline bool IsLeaf() { return (m_pLeftChild == nullptr) && (m_pRightChild == nullptr); }

		void AddLeftChild(TriangleTreeNode<T>* pLeftChild)
		{
			if (pLeftChild->m_pParent != nullptr)
			{
				if (pLeftChild == pLeftChild->m_pParent->m_pLeftChild)
				{
					pLeftChild->m_pParent->m_pLeftChild = nullptr;
				}
				if (pLeftChild == pLeftChild->m_pParent->m_pRightChild)
				{
					pLeftChild->m_pParent->m_pRightChild = nullptr;
				}
			}
			pLeftChild->m_pParent = this;

			if (m_pLeftChild != nullptr)
			{
				m_pLeftChild->m_pParent = nullptr;
				delete m_pLeftChild;
			}
			m_pLeftChild = pLeftChild;
		}

		void AddRightChild(TriangleTreeNode<T>* pRightChild)
		{
			if (pRightChild->m_pParent != nullptr)
			{
				if (pRightChild == pRightChild->m_pParent->m_pLeftChild)
				{
					pRightChild->m_pParent->m_pLeftChild = nullptr;
				}
				if (pRightChild == pRightChild->m_pParent->m_pRightChild)
				{
					pRightChild->m_pParent->m_pRightChild = nullptr;
				}
			}
			pRightChild->m_pParent = this;

			if (m_pRightChild != nullptr)
			{
				m_pRightChild->m_pParent = nullptr;
				delete m_pRightChild;
			}
			m_pRightChild = pRightChild;
		}

		void RemoveLeftChild()
		{
			if (m_pLeftChild != nullptr)
			{
				delete m_pLeftChild;
				m_pLeftChild = nullptr;
			}
		}

		void RemoveRightChild()
		{
			if (m_pRightChild != nullptr)
			{
				delete m_pRightChild;
				m_pRightChild = nullptr;
			}
		}

		void Populate(const vector<Vec3>& vertices, const vector<int>& indices, int from, int to)
		{
			if (to - from > 2)
			{
				TriangleTreeNode<int>* pLeftChild = m_pTree->CreateNode(this, true);
				((TriangleTreeNode<T>*)pLeftChild)->Populate(vertices, indices, from, from + (int)((to - from) * 0.5f));
				TriangleTreeNode<int>* pRightChild = m_pTree->CreateNode(this, false);
				((TriangleTreeNode<T>*)pRightChild)->Populate(vertices, indices, from + (int)((to - from) * 0.5f) + 1, to);
			}
			else
			{
				float minY = FLT_MAX;

				for (int i = from; i <= to; i++)
				{
					m_elements.push_back(i);

					if (vertices[i].y() < minY)
					{
						m_minYIndex = i;
						minY = vertices[i].y();
					}
				}

				if (m_elements.size() > 2)
				{
					m_hullEdges.push_back(make_tuple(m_elements[0], m_elements[1]));
					m_hullEdges.push_back(make_tuple(m_elements[1], m_elements[2]));
					m_hullEdges.push_back(make_tuple(m_elements[2], m_elements[0]));

					m_triangleIndices.push_back(make_tuple(m_elements[0], m_elements[1], m_elements[2]));
				}
				else
				{
					m_hullEdges.push_back(make_tuple(m_elements[0], m_elements[1]));
				}
			}
		}

		void MergeChild(const vector<Vec3>& vertices, const vector<int>& indices)
		{
			float minY = FLT_MAX;

			if (m_pLeftChild != nullptr && m_pRightChild != nullptr)
			{
				((TriangleTreeNode<T>*)m_pLeftChild)->MergeChild(vertices, indices);
				((TriangleTreeNode<T>*)m_pRightChild)->MergeChild(vertices, indices);
			
				if (m_pLeftChild->IsLeaf())
				{
					for (auto& i : m_pLeftChild->GetElements())
					{
						m_elements.push_back(i);

						if (vertices[i].y() < minY)
						{
							m_minYIndex = i;
							minY = vertices[i].y();
						}
					}

					for (auto& ii : m_pLeftChild->GetHullEdges())
					{
						m_hullEdges.push_back(ii);
					}

					for (auto& iii : m_pLeftChild->GetTriangleIndices())
					{
						m_triangleIndices.push_back(iii);
					}
				}

				if (m_pRightChild->IsLeaf())
				{
					for (auto& i : m_pRightChild->GetElements())
					{
						m_elements.push_back(i);

						if (vertices[i].y() < minY)
						{
							m_minYIndex = i;
							minY = vertices[i].y();
						}
					}

					for (auto& ii : m_pRightChild->GetHullEdges())
					{
						m_hullEdges.push_back(ii);
					}

					for (auto& iii : m_pRightChild->GetTriangleIndices())
					{
						m_triangleIndices.push_back(iii);
					}
				}

				tuple<int, int> currentEdge = make_tuple(m_pLeftChild->GetMinYIndex(), m_pRightChild->GetMinYIndex());

				auto& le = m_pLeftChild->FindHullEdges(get<0>(currentEdge));
				auto& re = m_pRightChild->FindHullEdges(get<1>(currentEdge));

				

//				m_triangleIndices.push_back(make_tuple(get<0>(ii), m_pRightChild->GetMinYIndex(), get<1>(ii)));
			}

#pragma region Remove Children
			if (m_pLeftChild != nullptr)
			{
				if (m_pLeftChild->IsLeaf())
				{
					RemoveLeftChild();
				}
			}

			if (m_pRightChild != nullptr)
			{
				if (m_pRightChild->IsLeaf())
				{
					RemoveRightChild();
				}
			}
#pragma endregion
		}

		void GetHullEdgesRecursive(vector<tuple<int, int>>& hullEdges)
		{
			if (m_pLeftChild)
			{
				m_pLeftChild->GetHullEdgesRecursive(hullEdges);
			}

			if (m_pRightChild)
			{
				m_pRightChild->GetHullEdgesRecursive(hullEdges);
			}

			for (auto& ii : m_hullEdges)
			{
				hullEdges.push_back(ii);
			}
		}

		void GetTriangleIndicesRecursive(vector<tuple<int, int, int>>& triangleIndices)
		{
			if (m_pLeftChild)
			{
				m_pLeftChild->GetTriangleIndicesRecursive(triangleIndices);
			}

			if (m_pRightChild)
			{
				m_pRightChild->GetTriangleIndicesRecursive(triangleIndices);
			}

			for (auto& iii : m_triangleIndices)
			{
				triangleIndices.push_back(iii);
			}
		}

		const tuple<int, int>& FindHullEdges(int index) const
		{
			for (auto& ii : m_hullEdges)
			{
				if (get<0>(ii) == index || get<1>(ii) == index)
					return ii;
			}
			return make_tuple(-1, -1);
		}

		inline int GetMinYIndex() const { return m_minYIndex; }
		inline const list<tuple<int, int>>& GetHullEdges() const { return m_hullEdges; }
		inline const list<tuple<int, int, int>> GetTriangleIndices() const { return m_triangleIndices; }

	protected:
		string m_name;
		vector<T> m_elements;

		TriangleTree<T>* m_pTree = nullptr;
		TriangleTreeNode<T>* m_pParent = nullptr;
		TriangleTreeNode<T>* m_pLeftChild = nullptr;
		TriangleTreeNode<T>* m_pRightChild = nullptr;

		int m_minYIndex = -1;
		list<tuple<int, int>> m_hullEdges;
		list<tuple<int, int, int>> m_triangleIndices;

		TriangleTreeNode(TriangleTree<T>* pTree, TriangleTreeNode<T>* pParent, const string& name)
			: m_pTree(pTree), m_pParent(pParent), m_name(name)
		{
		}

		virtual ~TriangleTreeNode()
		{
			if (m_pLeftChild != nullptr)
			{
				delete m_pLeftChild;
				m_pLeftChild = nullptr;
			}

			if (m_pRightChild != nullptr)
			{
				delete m_pRightChild;
				m_pRightChild = nullptr;
			}
		}

	public:
		friend class TriangleTree<T>;
	};

	template<typename T>
	class TriangleTree
	{
	public:
		TriangleTree()
		{
			m_pRootNode = new TriangleTreeNode<T>(this, nullptr, "Root");
		}

		inline TriangleTreeNode<T>* GetRootNode() { return m_pRootNode; }

		TriangleTreeNode<T>* CreateNode(TriangleTreeNode<T>* pParent, bool bLeftChild, const string& name = "")
		{
			if (pParent == nullptr)
			{
				return nullptr;
			}

			string nodeName(name);
			if (nodeName.empty())
			{
				char buffer[32];
				memset(buffer, 0, 32);
				itoa(m_nodeCount++, buffer, 10);
				nodeName = buffer;
			}

			auto pNode = new TriangleTreeNode<T>(this, pParent, nodeName);
			if (bLeftChild)
			{
				pParent->AddLeftChild(pNode);
			}
			else
			{
				pParent->AddRightChild(pNode);
			}

			return pNode;
		}

		void Populate(const vector<Vec3>& vertices, const vector<int>& indices, int from, int to)
		{
			((TriangleTreeNode<T>*)m_pRootNode)->Populate(vertices, indices, from, to);
		}

		void MergeChildNodes(const vector<Vec3>& vertices, const vector<int>& indices)
		{
			((TriangleTreeNode<T>*)m_pRootNode)->MergeChild(vertices, indices);
		}

		void GetHullEdges(vector<tuple<int, int>>& hullEdges)
		{
			m_pRootNode->GetHullEdgesRecursive(hullEdges);
		}

		void GetTriangles(vector<tuple<int, int, int>>& triangleIndices)
		{
			m_pRootNode->GetTriangleIndicesRecursive(triangleIndices);
		}

	protected:
		TriangleTreeNode<T>* m_pRootNode = nullptr;
		int m_nodeCount = 0;
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

void PrintNode(TriangleTreeNode<int>* pNode)
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
			printf("\nNode Name : %s\n", pNode->GetName().c_str());
			printf("min y index : %d\n", ((TriangleTreeNode<int>*)pNode)->GetMinYIndex());
			printf("Hull Edge Size : %d\n", ((TriangleTreeNode<int>*)pNode)->GetHullEdges().size());
			printf("Triangles Size : %d\n", ((TriangleTreeNode<int>*)pNode)->GetTriangleIndices().size());

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



	//Vec3 fv0(0, -400, 0);
	//Vec3 fv1(500, -400, 0);
	//Vec3 fv2(250, 0, 0);
	//pvd->AddTriangle(fv0, fv1, fv2, Vec4(GREEN, 0.3), true);

	vector<Vec3> vertices;
	//////////vertices.push_back(fv0);
	//////////vertices.push_back(fv1);
	//////////vertices.push_back(fv2);
	////////vertices.push_back(Vec3(110, -260, 0));
	////////vertices.push_back(Vec3(120, -330, 0));
	////////vertices.push_back(Vec3(190, -120, 0));
	////////vertices.push_back(Vec3(230, -180, 0));
	////////vertices.push_back(Vec3(170, -230, 0));
	////////vertices.push_back(Vec3(200, -250, 0));
	////////vertices.push_back(Vec3(150, -310, 0));
	////////vertices.push_back(Vec3(230, -300, 0));
	////////vertices.push_back(Vec3(230, -210, 0));
	////////vertices.push_back(Vec3(270, -190, 0));
	////////vertices.push_back(Vec3(240, -130, 0));
	////////vertices.push_back(Vec3(230, -70, 0));
	////////vertices.push_back(Vec3(280, -100, 0));
	////////vertices.push_back(Vec3(310, -210, 0));
	////////vertices.push_back(Vec3(240, -250, 0));
	////////vertices.push_back(Vec3(270, -270, 0));
	////////vertices.push_back(Vec3(290, -230, 0));
	////////vertices.push_back(Vec3(330, -240, 0));
	////////vertices.push_back(Vec3(340, -270, 0));
	////////vertices.push_back(Vec3(320, -320, 0));
	////////vertices.push_back(Vec3(130, -360, 0));
	////////vertices.push_back(Vec3(390, -380, 0));
	////////vertices.push_back(Vec3(270, -350, 0));
	////////vertices.push_back(Vec3(380, -330, 0));
	////////vertices.push_back(Vec3(360, -220, 0));




	vertices.push_back(Vec3(0.939718, 0.00149083, 0));
	vertices.push_back(Vec3(0.459635, 0.328918, 0));
	vertices.push_back(Vec3(0.545203, 0.521633, 0));
	vertices.push_back(Vec3(0.142404, 0.982294, 0));
	vertices.push_back(Vec3(0.733818, 0.0352899, 0));
	vertices.push_back(Vec3(0.150613, 0.903548, 0));
	vertices.push_back(Vec3(0.636916, 0.557598, 0));
	vertices.push_back(Vec3(0.0998009, 0.48953, 0));
	vertices.push_back(Vec3(0.424942, 0.925505, 0));
	vertices.push_back(Vec3(0.441789, 0.623109, 0));
	vertices.push_back(Vec3(0.40419, 0.765702, 0));
	vertices.push_back(Vec3(0.472585, 0.453032, 0));
	vertices.push_back(Vec3(0.464472, 0.764211, 0));
	vertices.push_back(Vec3(0.0129508, 0.124114, 0));
	vertices.push_back(Vec3(0.919269, 0.242578, 0));
	vertices.push_back(Vec3(0.870547, 0.14182, 0));
	vertices.push_back(Vec3(0.185451, 0.207288, 0));
	vertices.push_back(Vec3(0.719935, 0.238272, 0));
	vertices.push_back(Vec3(0.548535, 0.64969, 0));
	vertices.push_back(Vec3(0.620134, 0.748742, 0));
	vertices.push_back(Vec3(0.123593, 0.724185, 0));
	vertices.push_back(Vec3(0.178345, 0.125632, 0));
	vertices.push_back(Vec3(0.719403, 0.958484, 0));
	vertices.push_back(Vec3(0.705759, 0.6726, 0));
	vertices.push_back(Vec3(0.254932, 0.194273, 0));
	vertices.push_back(Vec3(0.692809, 0.548486, 0));
	vertices.push_back(Vec3(0.335663, 0.951695, 0));
	vertices.push_back(Vec3(0.822261, 0.406666, 0));
	vertices.push_back(Vec3(0.150212, 0.744407, 0));
	vertices.push_back(Vec3(0.102327, 0.168394, 0));
	vertices.push_back(Vec3(0.601678, 0.0947169, 0));
	vertices.push_back(Vec3(0.482193, 0.419652, 0));
	vertices.push_back(Vec3(0.478085, 0.370531, 0));
	vertices.push_back(Vec3(0.303848, 0.29402, 0));
	vertices.push_back(Vec3(0.758682, 0.412048, 0));
	vertices.push_back(Vec3(0.598089, 0.62142, 0));
	vertices.push_back(Vec3(0.50375, 0.217775, 0));
	vertices.push_back(Vec3(0.90528, 0.0729336, 0));
	vertices.push_back(Vec3(0.168087, 0.26608, 0));
	vertices.push_back(Vec3(0.0830191, 0.666267, 0));
	vertices.push_back(Vec3(0.0178743, 0.521673, 0));
	vertices.push_back(Vec3(0.980692, 0.497873, 0));
	vertices.push_back(Vec3(0.416196, 0.426957, 0));
	vertices.push_back(Vec3(0.498499, 0.078221, 0));
	vertices.push_back(Vec3(0.938112, 0.0564251, 0));
	vertices.push_back(Vec3(0.194651, 0.784201, 0));
	vertices.push_back(Vec3(0.17943, 0.644377, 0));
	vertices.push_back(Vec3(0.596562, 0.162781, 0));
	vertices.push_back(Vec3(0.67568, 0.426602, 0));
	vertices.push_back(Vec3(0.691281, 0.0898479, 0));
	vertices.push_back(Vec3(0.507594, 0.160522, 0));
	vertices.push_back(Vec3(0.608262, 0.42358, 0));
	vertices.push_back(Vec3(0.489719, 0.638848, 0));
	vertices.push_back(Vec3(0.62757, 0.925707, 0));
	vertices.push_back(Vec3(0.0735229, 0.211892, 0));
	vertices.push_back(Vec3(0.129071, 0.847486, 0));
	vertices.push_back(Vec3(0.135411, 0.155467, 0));
	vertices.push_back(Vec3(0.93442, 0.0632851, 0));
	vertices.push_back(Vec3(0.955981, 0.511089, 0));
	vertices.push_back(Vec3(0.337858, 0.900504, 0));
	vertices.push_back(Vec3(0.280301, 0.0844871, 0));
	vertices.push_back(Vec3(0.646577, 0.810656, 0));
	vertices.push_back(Vec3(0.772707, 0.923965, 0));
	vertices.push_back(Vec3(0.0383153, 0.387075, 0));
	vertices.push_back(Vec3(0.282988, 0.285117, 0));
	vertices.push_back(Vec3(0.410746, 0.461368, 0));
	vertices.push_back(Vec3(0.209465, 0.0732255, 0));
	vertices.push_back(Vec3(0.281675, 0.613882, 0));
	vertices.push_back(Vec3(0.0740536, 0.917759, 0));
	vertices.push_back(Vec3(0.347255, 0.550597, 0));
	vertices.push_back(Vec3(0.118073, 0.40667, 0));
	vertices.push_back(Vec3(0.0093963, 0.650093, 0));
	vertices.push_back(Vec3(0.837772, 0.322183, 0));
	vertices.push_back(Vec3(0.362819, 0.839438, 0));
	vertices.push_back(Vec3(0.065065, 0.398217, 0));
	vertices.push_back(Vec3(0.324504, 0.452362, 0));
	vertices.push_back(Vec3(0.782077, 0.1131, 0));
	vertices.push_back(Vec3(0.913758, 0.990994, 0));
	vertices.push_back(Vec3(0.572613, 0.0398746, 0));
	vertices.push_back(Vec3(0.632083, 0.377112, 0));
	vertices.push_back(Vec3(0.498559, 0.122116, 0));
	vertices.push_back(Vec3(0.284828, 0.826515, 0));
	vertices.push_back(Vec3(0.380486, 0.715446, 0));
	vertices.push_back(Vec3(0.275432, 0.176422, 0));
	vertices.push_back(Vec3(0.542714, 0.393263, 0));
	vertices.push_back(Vec3(0.912613, 0.336984, 0));
	vertices.push_back(Vec3(0.477649, 0.995046, 0));
	vertices.push_back(Vec3(0.58811, 0.884622, 0));
	vertices.push_back(Vec3(0.695572, 0.881946, 0));
	vertices.push_back(Vec3(0.674352, 0.893628, 0));
	vertices.push_back(Vec3(0.122959, 0.842071, 0));
	vertices.push_back(Vec3(0.0422684, 0.516516, 0));
	vertices.push_back(Vec3(0.6244, 0.719956, 0));
	vertices.push_back(Vec3(0.75744, 0.690001, 0));
	vertices.push_back(Vec3(0.243914, 0.00450979, 0));
	vertices.push_back(Vec3(0.482008, 0.513579, 0));
	vertices.push_back(Vec3(0.7012, 0.611247, 0));
	vertices.push_back(Vec3(0.569394, 0.176595, 0));
	vertices.push_back(Vec3(0.223551, 0.616201, 0));
	vertices.push_back(Vec3(0.981285, 0.291973, 0));

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

	auto pMesh = g_pMP->GetOrCreateMesh("Triangle");

	Vec3 fv0(-500, -500, 0);
	Vec3 fv1(500, -500, 0);
	Vec3 fv2(0, 500, 0);
	pMesh->GetOrCreateFace(fv0, fv1, fv2);

	for (auto& v : vertices)
	{
		pMesh->InsertVertex(v * 100);
	}

	//pMesh->Refresh();

	//for (auto& pF : pMesh->GetFaces())
	//{
	//	pvd->AddTriangle(pF->V0()->P() + Vec3(0, 0, 10), pF->V1()->P() + Vec3(0, 0, 10), pF->V2()->P() + Vec3(0, 0, 10), V4_BLUE, true);
	//}

	pMesh->RefineFaces();

	auto pV0 = pMesh->GetVertex(fv0);
	auto pV1 = pMesh->GetVertex(fv1);
	auto pV2 = pMesh->GetVertex(fv2);
	pV0->QueryDelete();
	pV1->QueryDelete();
	pV2->QueryDelete();

	g_pMP->UpdateModel();

	for (int i = 1; i < vertices.size(); i++)
	{
		auto pE = pMesh->GetEdge(vertices[i - 1], vertices[i]);
		if (pE == nullptr)
		{
			pvd->AddLine(vertices[i - 1], vertices[i], V4_RED, V4_RED);
		}
	}

	return viewer.run();
}
