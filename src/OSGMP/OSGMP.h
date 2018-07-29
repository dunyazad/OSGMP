#pragma once

#include "../OSGCommon.h"
using namespace osg;

#include "./Algorithm/Intersections.h"

#include "StopWatch.h"

#include "PrimitiveRendererVEFM.h"
#include "File.h"

#include "./VEFM/VEFM.h"
#include "./VEFM/VoxelSpace.h"
using namespace VEFM;

//#define TtoVec3(v) Vec3(v.x.Float(), v.y.Float(), v.z.Float())
#define Vec3toT(v) T(v.x(), v.y(), v.z())

#include "./Algorithm/tri_tri_intersection.h"
#include "./Algorithm/tri_aabb_intersection.h"

#define MIN_Vec3(a, b) a.x() = MIN(a.x(), b.x()); a.y() = MIN(a.y(), b.y()); a.z() = MIN(a.z(), b.z());
#define MAX_Vec3(a, b) a.x() = MAX(a.x(), b.x()); a.y() = MAX(a.y(), b.y()); a.z() = MAX(a.z(), b.z());

typedef unsigned int uint;

template<typename T>
class OSGMP
{
public:
	OSGMP(Group* pRootNode);
	~OSGMP();

	Mesh<T>* GetMesh(const string& name);
	Mesh<T>* GetOrCreateMesh(const string& name);
	
	bool LoadABDFile(const string& name, const string& fileName);
	bool SaveABDFile(const string& name, const string& filename);
	
	bool LoadABDEFile(const string& name, const string& fileName, int* iReservedDataSize, void** pReservedData);
	bool SaveABDEFile(const string& name, const string& filename, int iReservedDataSize, void* pReservedData);

	bool LoadSTLFile(const string& name, const string& filename);
	bool SaveSTLFile(const string& name, const string& filename, bool bianry);
	
	bool LoadOFFFile(const string& name, const string& fileName);
	bool SaveOFFFile(const string& name, const string& filename);
	
	bool GetOFFString(const string& name, stringstream& ss);

	void UpdateModel();
	void UpdateModel(const string& name);
	
	void TogglePolygonMode();
	void TogglePolygonMode(const string& name);
	
	void ToggleVisible();
	void ToggleVisible(const string& name);

	bool CheckIntersection(const string& nameA, const string& nameB, map<tuple<Mesh<T>*, Face<T>*>, set<T>>& result);
	bool CheckIntersections(const string& baseName, const vector<string>& names, map<tuple<Mesh<T>*, Face<T>*>, set<pair<T, T>>>& result);
	
	void SplitFaces(const map<tuple<Mesh<T>*, Face<T>*>, set<T>>& input);

	void Triangulate(const T& fv0, const T& fv1, const T& fv2, const vector<T>& points, vector<tuple<T, T, T>>& result);

	//void ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint);
	//void ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints);
	//void ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint);
	//void ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints);

	inline void SetVD(PrimitiveRendererVEFM<T>* pVD) { m_pVD = pVD; }

private:
	ref_ptr<Group> m_pRootNode;

	map<string, Mesh<T>*> m_meshes;
	map<string, ref_ptr<Group>> m_models;

	PrimitiveRendererVEFM<T>* m_pVD = nullptr;
};

template<typename T> void MakeReverse(vector<pair<T, T>>& input);
template<typename T> void SortPairs(const vector<pair<T, T>>& input, vector<vector<T>>& result);

#include "OSGMP.hpp"