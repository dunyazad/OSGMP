#pragma once

#include "VEFM_Common.h"
#include "../Algorithm/tri_aabb_intersection.h"

namespace VEFM
{
	inline bool CheckTriangleAABBIntersection(const Vec3& t0, const Vec3& t1, const Vec3& t2, const Vec3& vmin, const Vec3& vmax)
	{
		auto vcenter = (vmax + vmin) * 0.5f;
		auto delta = vmax - vmin;
		float tvs[3][3] = { { t0.x(), t0.y(), t0.z() }, { t1.x(), t1.y(), t1.z() }, { t2.x(), t2.y(), t2.z() } };
		return triBoxOverlap(&(vcenter.x()), &(delta.x()), tvs) == 1;
	}

	template<typename T, typename U>
	Voxel<T, U>::Voxel(const U& min, const U& max)
		: m_min(min), m_center((min + max) * 0.5f), m_max(max)
	{
	}

	template<typename T, typename U>
	void Voxel<T, U>::Insert(T* pT)
	{
		m_pElements.insert(pT);
	}

	template<typename T, typename U>
	VoxelSpace<T, U>::VoxelSpace(float voxelUnit)
		: m_voxelUnit(voxelUnit)
	{
	}

	template<typename T, typename U>
	VoxelSpace<T, U>::~VoxelSpace()
	{

	}

	template<typename T, typename U>
	void VoxelSpace<T, U>::GetIndex(const U& position, tuple<int, int, int>& index)
	{
		int xIndex = (int)(position.x() / m_voxelUnit);
		int yIndex = (int)(position.y() / m_voxelUnit);
		int zIndex = (int)(position.z() / m_voxelUnit);

		if (position.x() < 0) xIndex -= 1;
		if (position.y() < 0) yIndex -= 1;
		if (position.z() < 0) zIndex -= 1;

		index = make_tuple(xIndex, yIndex, zIndex);
	}

	template<typename T, typename U>
	void VoxelSpace<T, U>::InsertPoint(const U& v, T* pV)
	{
		tuple<int, int, int> index;
		GetIndex(v, index);

		int x = get<0>(index);
		int y = get<1>(index);
		int z = get<2>(index);

		float minx = x; float miny = y; float minz = z;
		float maxx = x + 1; float maxy = y + 1; float maxz = z + 1;

		T vmin(minx * m_voxelUnit, miny * m_voxelUnit, minz * m_voxelUnit);
		T vmax(maxx * m_voxelUnit, maxy * m_voxelUnit, maxz * m_voxelUnit);

		if (m_voxels.count(index) == 0)
		{
			MIN_Vec3(m_min, vmin);
			MAX_Vec3(m_max, vmax);

			Voxel<T, U>* pVoxel = new Voxel<T, U>(vmin, vmax);
			m_voxels[index] = pVoxel;
		}
		m_voxels[index]->Insert(pT);
	}

	template<typename T, typename U>
	void VoxelSpace<T, U>::InsertTriangle(const U& v0, const U& v1, const U& v2, T* pT)
	{
		AABB<U> aabb;
		aabb.Insert(v0);
		aabb.Insert(v1);
		aabb.Insert(v2);

		auto& tmin = aabb.min;
		auto& tmax = aabb.max;

		tuple<int, int, int> minIndex;
		GetIndex(tmin, minIndex);

		tuple<int, int, int> maxIndex;
		GetIndex(tmax, maxIndex);

		for (int z = get<2>(minIndex); z <= get<2>(maxIndex); z++)
		{
			for (int y = get<1>(minIndex); y <= get<1>(maxIndex); y++)
			{
				for (int x = get<0>(minIndex); x <= get<0>(maxIndex); x++)
				{
					float minx = x; float miny = y; float minz = z;
					float maxx = x + 1; float maxy = y + 1; float maxz = z + 1;

					U vmin(minx * m_voxelUnit, miny * m_voxelUnit, minz * m_voxelUnit);
					U vmax(maxx * m_voxelUnit, maxy * m_voxelUnit, maxz * m_voxelUnit);

					if (CheckTriangleAABBIntersection(v0, v1, v2, vmin, vmax))
					{
						auto& index = make_tuple(x, y, z);
						if (m_voxels.count(index) == 0)
						{
							MIN_Vec3(m_min, vmin);
							MAX_Vec3(m_max, vmax);

							Voxel<T, U>* pVoxel = new Voxel<T, U>(vmin, vmax);
							m_voxels[index] = pVoxel;
						}
						m_voxels[index]->Insert(pT);
					}
				}
			}
		}
	}
}
