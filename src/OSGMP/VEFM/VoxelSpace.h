#pragma once

#include "VEFM_Common.h"
#include "../Algorithm/tri_aabb_intersection.h"

namespace VEFM
{
	inline bool CheckTriangleAABBIntersection(const Vec3& t0, const Vec3& t1, const Vec3& t2, const Vec3& vmin, const Vec3& vmax);

	template<typename T, typename U>
	class Voxel
	{
	public:
		Voxel(const U& min, const U& max);

		void Insert(T* pT);

		inline const U& GetMin() const { return m_min; }
		inline const U& GetCenter() const { return m_center; }
		inline const U& GetMax() const { return m_max; }

		const set<T*>& GetElements() const { return m_pElements; }

	private:
		U m_min = U(FLT_MAX, FLT_MAX, FLT_MAX);
		U m_center = U();
		U m_max = U(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		set<T*> m_pElements;
	};

	template<typename T, typename U>
	class VoxelSpace
	{
	public:
		VoxelSpace(float voxelUnit);
		~VoxelSpace();

		void GetIndex(const U& position, tuple<int, int, int>& index);
		void InsertPoint(const U& v, T* pV);
		void InsertTriangle(const U& v0, const U& v1, const U& v2, T* pT);

		inline const U& GetMin() const { return m_min; }
		inline const U& GetCenter() const { return (m_min + m_max) * 0.5f; }
		inline const U& GetMax() const { return m_max; }

		inline Voxel<T, U>* GetVoxel(const tuple<int, int, int>& index) { if (m_voxels.count(index) != 0) return m_voxels[index]; else return nullptr; }

		inline const map<tuple<int, int, int>, Voxel<T, U>*>& GetVoxels() const { return m_voxels; }

	private:
		U m_min = U(FLT_MAX, FLT_MAX, FLT_MAX);
		U m_max = U(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		float m_voxelUnit = 10.0f;
		
		map<tuple<int, int, int>, Voxel<T, U>*> m_voxels;
	};
}

#include "VoxelSpace.hpp"