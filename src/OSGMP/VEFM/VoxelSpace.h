#pragma once

#include "VEFM_Common.h"
#include "../Algorithm/tri_aabb_intersection.h"

namespace VEFM
{
	inline bool CheckIntersection(const V3& t0, const V3& t1, const V3& t2, const V3& vmin, const V3& vmax)
	{
		auto vcenter = (vmax + vmin) * 0.5f;
		auto delta = vmax - vmin;
		float tvs[3][3] = { { t0.x, t0.y, t0.z }, { t1.x, t1.y, t1.z }, { t2.x, t2.y, t2.z } };
		return triBoxOverlap(vcenter.elements, delta.elements, tvs) == 1;
	}

	template<typename T>
	class Voxel
	{
	public:
		Voxel(const V3& min, const V3& max)
			: m_min(min), m_center((min + max) * 0.5f), m_max(max)
		{
		}

		void Insert(T* pT)
		{
			m_pElements.insert(pT);
		}

		inline const V3& GetMin() const { return m_min; }
		inline const V3& GetCenter() const { return m_center; }
		inline const V3& GetMax() const { return m_max; }

		const set<T*>& GetElements() const { return m_pElements; }

	private:
		V3 m_min = V3::Max();
		V3 m_center = V3();
		V3 m_max = V3::Min();

		set<T*> m_pElements;
	};

	template<typename T>
	class VoxelSpace
	{
	public:
		VoxelSpace(float voxelUnit)
			: m_voxelUnit(voxelUnit)
		{
		}

		~VoxelSpace()
		{

		}

		void GetIndex(const V3& position, tuple<int, int, int>& index)
		{
			int xIndex = (int)(position.x / m_voxelUnit);
			int yIndex = (int)(position.y / m_voxelUnit);
			int zIndex = (int)(position.z / m_voxelUnit);

			if (position.x < 0) xIndex -= 1;
			if (position.y < 0) yIndex -= 1;
			if (position.z < 0) zIndex -= 1;

			index = make_tuple(xIndex, yIndex, zIndex);
		}

		void InsertPoint(const V3& v, T* pV)
		{
			tuple<int, int, int> index;
			GetIndex(v, index);

			int x = get<0>(index);
			int y = get<1>(index);
			int z = get<2>(index);

			float minx = x; float miny = y; float minz = z;
			float maxx = x + 1; float maxy = y + 1; float maxz = z + 1;

			V3 vmin(minx * m_voxelUnit, miny * m_voxelUnit, minz * m_voxelUnit);
			V3 vmax(maxx * m_voxelUnit, maxy * m_voxelUnit, maxz * m_voxelUnit);

			if (m_voxels.count(index) == 0)
			{
				MIN_V3(m_min, vmin);
				MAX_V3(m_max, vmax);

				Voxel<T>* pVoxel = new Voxel<T>(vmin, vmax);
				m_voxels[index] = pVoxel;
			}
			m_voxels[index]->Insert(pT);
		}

		void InsertTriangle(const V3& v0, const V3& v1, const V3& v2, T* pT)
		{
			AABB aabb;
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

						V3 vmin(minx * m_voxelUnit, miny * m_voxelUnit, minz * m_voxelUnit);
						V3 vmax(maxx * m_voxelUnit, maxy * m_voxelUnit, maxz * m_voxelUnit);

						if (CheckIntersection(v0, v1, v2, vmin, vmax))
						{
							auto& index = make_tuple(x, y, z);
							if (m_voxels.count(index) == 0)
							{
								MIN_V3(m_min, vmin);
								MAX_V3(m_max, vmax);

								Voxel<T>* pVoxel = new Voxel<T>(vmin, vmax);
								m_voxels[index] = pVoxel;
							}
							m_voxels[index]->Insert(pT);
						}
					}
				}
			}
		}

		inline const V3& GetMin() const { return m_min; }
		inline const V3& GetCenter() const { return (m_min + m_max) * 0.5f; }
		inline const V3& GetMax() const { return m_max; }

		inline Voxel<T>* GetVoxel(const tuple<int, int, int>& index) { if (m_voxels.count(index) != 0) return m_voxels[index]; else return nullptr; }

		inline const map<tuple<int, int, int>, Voxel<T>*>& GetVoxels() const { return m_voxels; }

	private:
		V3 m_min = V3::Max();
		V3 m_max = V3::Min();

		float m_voxelUnit = 10.0f;
		
		map<tuple<int, int, int>, Voxel<T>*> m_voxels;
	};

}
