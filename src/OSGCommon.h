#pragma once

#include <osg/Geode>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Material>
#include <osg/PolygonOffset>
#include <osg/PolygonMode>
#include <osg/LineSegment>
#include <osg/LineStipple>
#include <osg/TriangleFunctor>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/io_utils>
#include <osg/CullFace>
#include <osg/BlendFunc>

#include <osgDB/Readfile>
#include <osgDB/Registry>

#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/OrbitManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/StateSetManipulator>

#include <osgFX/Scribe>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>
#include <osgManipulator/AntiSquish>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>

#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Optimizer>
#include <osgUtil/DelaunayTriangulator>
#include <osgUtil/EdgeCollector>

#ifdef _DEBUG
#pragma comment(lib, "OpenThreadsd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgFXd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgManipulatord.lib")
#pragma comment(lib, "osgParticled.lib")
#pragma comment(lib, "osgPresentationd.lib")
#pragma comment(lib, "osgShadowd.lib")
#pragma comment(lib, "osgSimd.lib")
#pragma comment(lib, "osgTerraind.lib")
#pragma comment(lib, "osgTextd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgVolumed.lib")
#pragma comment(lib, "osgWidgetd.lib")
#else
#pragma comment(lib, "OpenThreads.lib")
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgAnimation.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgFX.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgManipulator.lib")
#pragma comment(lib, "osgParticle.lib")
#pragma comment(lib, "osgPresentation.lib")
#pragma comment(lib, "osgShadow.lib")
#pragma comment(lib, "osgSim.lib")
#pragma comment(lib, "osgTerrain.lib")
#pragma comment(lib, "osgText.lib")
#pragma comment(lib, "osgUtil.lib")
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgVolume.lib")
#pragma comment(lib, "osgWidget.lib")
#endif






#define A_CONTAINS_Point_Vec3(min, max, point) (min.x() < point.x() && min.y() < point.y() && min.z() < point.z()) && (max.x() > point.x() && max.y() > point.y() && max.z() > point.z())
#define A_CONTAINS_Point_Trivertex(min, max, point) (min.x() < point->x() && min.y() < point->y() && min.z() < point->z()) && (max.x() > point->x() && max.y() > point->y() && max.z() > point->z())
#define A_CONTAINS_B_Vec3(amin, amax, bmin, bmax) (amin.x() < bmin.x() && amin.y() < bmin.y() && amin.z() < bmin.z()) && (amax.x() > bmax.x() && amax.y() > bmax.y() && amax.z() > bmax.z())




struct RAW_DATA_INFO
{
	bool bSetInfo;
	int iVertexNum;
	osg::Vec3 * pVertexPtr;
	int iIndicesNum;
	int * pIndicesPtr;
};



#define BLACK	0.0f, 0.0f, 0.0f
#define RED		1.0f, 0.0f, 0.0f
#define GREEN	0.0f, 1.0f, 0.0f
#define BLUE	0.0f, 0.0f, 1.0f
#define WHITE	1.0f, 1.0f, 1.0f
#define YELLOW	1.0f, 1.0f, 0.0f
#define MAGENTA	1.0f, 0.0f, 1.0f
#define CYAN	0.0f, 1.0f, 1.0f
#define GRAY	0.5f, 0.5f, 0.5f
