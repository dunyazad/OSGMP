#pragma once

#define XY(a) a.x, a.y
#define XY_(a) a->x, a->y
#define XYZ(a) a.x, a.y, a.z
#define XYZ_(a) a->x, a->y, a->z
#define XYZW(a) a.x, a.y, a.z, a.w
#define XYZW_(a) a->x, a->y, a->z, a->w

#undef MIN
#define MIN(a, b) (a) < (b) ? (a) : (b)

#undef MAX
#define MAX(a, b) (a) > (b) ? (a) : (b)

#define MIN_V3(a, b) a.x = MIN(a.x, b.x); a.y = MIN(a.y, b.y); a.z = MIN(a.z, b.z);
#define MAX_V3(a, b) a.x = MAX(a.x, b.x); a.y = MAX(a.y, b.y); a.z = MAX(a.z, b.z);

#define CONTAINS_V3(min, max, p) (min.x <= p.x && min.y <= p.y && min.z <= p.z) && (max.x >= p.x && max.y >= p.y && max.z >= p.z)
#define A_CONTAINS_B_V3(amin, amax, bmin, bmax) (amin.x < bmin.x && amin.y < bmin.y && amin.z < bmin.z) && (amax.x > bmax.x && amax.y > bmax.y && amax.z > bmax.z)

namespace VEFM
{
	struct V3
	{
		union
		{
			float elements[3];
			struct
			{
				float x;
				float y;
				float z;
			};
		};

		V3() : x(0.0f), y(0.0f), z(0.0f) {}
		V3(float x, float y, float z) : x(x), y(y), z(z) {}
		V3(const V3& other) : x(other.x), y(other.y), z(other.z) {}

		inline V3 operator - () { return V3(-x, -y, -z); }

		inline V3 operator + (float scalar) { return V3(x + scalar, y + scalar, z + scalar); }
		inline V3 operator - (float scalar) { return V3(x - scalar, y - scalar, z - scalar); }
		inline V3 operator * (float scalar) { return V3(x * scalar, y * scalar, z * scalar); }
		inline V3 operator / (float scalar) { return V3(x / scalar, y / scalar, z / scalar); }

		inline void operator += (float scalar) { x += scalar; y += scalar; z += scalar; }
		inline void operator -= (float scalar) { x -= scalar; y -= scalar; z -= scalar; }
		inline void operator *= (float scalar) { x *= scalar; y *= scalar; z *= scalar; }
		inline void operator /= (float scalar) { x /= scalar; y /= scalar; z /= scalar; }

		inline V3 operator + (const V3& other) { return V3(x + other.x, y + other.y, z + other.z); }
		inline friend V3 operator + (const V3& a, const V3& b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }

		inline void operator += (const V3& other) { x += other.x; y += other.y; z += other.z; }
		inline void operator -= (const V3& other) { x -= other.x; y -= other.y; z -= other.z; }

		inline V3 operator - (const V3& other) { return V3(x - other.x, y - other.y, z - other.z); }
		inline friend V3 operator - (const V3& a, const V3& b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }

		//inline friend V3 operator * (const V3& a, float scalar) { return V3(a.x * scalar, a.y * scalar, a.z * scalar); }
		//inline friend V3 operator * (float scalar, const V3& a) { return V3(a.x * scalar, a.y * scalar, a.z * scalar); }

		//inline friend V3 operator / (const V3& a, float scalar) { return V3(a.x / scalar, a.y / scalar, a.z / scalar); }
		//inline friend V3 operator / (float scalar, const V3& a) { return V3(a.x / scalar, a.y / scalar, a.z / scalar); }

		inline bool operator == (const V3& other) const { return x == other.x && y == other.y && z == other.z; }
		inline bool operator != (const V3& other) const { return x != other.x || y != other.y || z != other.z; }

		inline bool operator < (const V3& other)
		{
			//if (FloatEqual(x, other.x) || x > other.x) return false;
			//else if (FloatEqual(y, other.y) || y > other.y) return false;
			//else if (FloatEqual(z, other.z) || z > other.z) return false;
			//else return true;

			if (x < other.x) return true;
			else if (x > other.x) return false;
			else if (y < other.y) return true;
			else if (y > other.y) return false;
			else return (z < other.z);
		}

		inline friend bool operator < (const V3& a, const V3& b)
		{
			//if (FloatEqual(a.x, b.x) || a.x > b.x) return false;
			//else if (FloatEqual(a.y, b.y) || a.y > b.y) return false;
			//else if (FloatEqual(a.z, b.z) || a.z > b.z) return false;
			//else return true;

			if (a.x < b.x) return true;
			else if (a.x > b.x) return false;
			else if (a.y < b.y) return true;
			else if (a.y > b.y) return false;
			else return (a.z < b.z);
		}

		inline bool operator >(const V3& other)
		{
			return !this->operator<(other);
		}

		inline friend bool operator > (const V3& a, const V3& b)
		{
			return !(a < b);
		}

		//inline float length() { return sqrtf(x * x + y * y + z * z); }
		//inline float lengthSquared() { return x * x + y * y + z * z; }

		inline float length() { return sqrtf(x * x + y * y + z * z); }
		inline float lengthSquared() { return x * x + y * y + z * z; }

		//inline float normalize()
		//{
		//	float norm = sqrtf(x * x + y * y + z * z);
		//	if (norm > 0.0f)
		//	{
		//		x = x / norm;
		//		y = y / norm;
		//		z = z / norm;
		//	}
		//	return norm;
		//}

		inline float normalize()
		{
			float norm = length();
			if (norm > 0.0f)
			{
				x = x / norm;
				y = y / norm;
				z = z / norm;
			}
			return norm;
		}

		inline float operator * (const V3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		inline const V3 operator ^ (const V3& other) const
		{
			return V3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		static float Length(const V3& v0, const V3& v1)
		{
			return (v1 - v0).length();
		}

		static float LengthSquared(const V3& v0, const V3& v1)
		{
			return (v1 - v0).lengthSquared();
		}

		static V3 Min()
		{
			return V3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		}

		static V3 Max()
		{
			return V3(FLT_MAX, FLT_MAX, FLT_MAX);
		}
	};

	struct V3V3
	{
		union
		{
			float elements[6];
			struct
			{
				V3 v0;
				V3 v1;
			};
		};

		V3V3() {}
		V3V3(const V3& v0, const V3& v1) : v0(v0), v1(v1) {}
		V3V3(const V3V3& other) : v0(other.v0), v1(other.v1) {}

		inline V3V3 operator - () { return V3V3(-v0, -v1); }

		inline V3V3 operator + (float scalar) { return V3V3(v0 + scalar, v1 + scalar); }
		inline V3V3 operator - (float scalar) { return V3V3(v0 - scalar, v1 - scalar); }
		inline V3V3 operator * (float scalar) { return V3V3(v0 * scalar, v1 * scalar); }
		inline V3V3 operator / (float scalar) { return V3V3(v0 / scalar, v1 / scalar); }

		inline void operator += (float scalar) { v0 += scalar; v1 += scalar; }
		inline void operator -= (float scalar) { v0 -= scalar; v1 -= scalar; }
		inline void operator *= (float scalar) { v0 *= scalar; v1 *= scalar; }
		inline void operator /= (float scalar) { v0 /= scalar; v1 /= scalar; }

		inline V3V3 operator + (const V3V3& other) { return V3V3(v0 + other.v0, v1 + other.v1); }
		inline friend V3V3 operator + (const V3V3& a, const V3V3& b) { return V3V3(a.v0 + b.v0, a.v1 + b.v1); }

		inline void operator += (const V3V3& other) { v0 += other.v0; v1 += other.v1; }
		inline void operator -= (const V3V3& other) { v0 -= other.v0; v1 -= other.v1; }

		inline V3V3 operator - (const V3V3& other) { return V3V3(v0 - other.v0, v1 - other.v1); }
		inline friend V3V3 operator - (const V3V3& a, const V3V3& b) { return V3V3(a.v0 - b.v0, a.v1 - b.v1); }

		//inline friend V3V3 operator * (const V3V3& a, float scalar) { return V3V3(a.v0 * scalar, a.v1 * scalar); }
		//inline friend V3V3 operator * (float scalar, const V3V3& a) { return V3V3(a.v0 * scalar, a.v1 * scalar); }

		//inline friend V3V3 operator / (const V3V3& a, float scalar) { return V3V3(a.v0 / scalar, a.v1 / scalar); }
		//inline friend V3V3 operator / (float scalar, const V3V3& a) { return V3V3(a.v0 / scalar, a.v1 / scalar); }

		inline bool operator == (const V3V3& other) const { return v0 == other.v0 && v1 == other.v1; }
		inline bool operator != (const V3V3& other) const { return v0 != other.v0 || v1 != other.v1; }

		inline bool operator < (const V3V3& other)
		{
			if (v0 < other.v0) return true;
			else if (v0 > other.v0) return false;
			else if (v1 < other.v1) return true;
			else return false;
		}

		inline friend bool operator < (const V3V3& a, const V3V3& b)
		{
			if (a.v0 < b.v0) return true;
			else if (a.v0 > b.v0) return false;
			else if (a.v1 < b.v1) return true;
			else return false;
		}

		inline bool operator >(const V3V3& other)
		{
			if (v0 > other.v0) return true;
			else if (v0 < other.v0) return false;
			else if (v1 > other.v1) return true;
			else if (v1 < other.v1) return false;
			else return false;
		}

		inline friend bool operator >(const V3V3& a, const V3V3& b)
		{
			if (a.v0 > b.v0) return true;
			else if (a.v0 < b.v0) return false;
			else if (a.v1 > b.v1) return true;
			else if (a.v1 < b.v1) return false;
			else return false;
		}
	};

	struct V3V3V3
	{
		union
		{
			float elements[9];
			struct
			{
				V3 v0;
				V3 v1;
				V3 v2;
			};
		};

		V3V3V3() {}
		V3V3V3(const V3& v0, const V3& v1, const V3& v2) : v0(v0), v1(v1), v2(v2) {}
		V3V3V3(const V3V3V3& other) : v0(other.v0), v1(other.v1), v2(other.v2) {}

		inline V3V3V3 operator - () { return V3V3V3(-v0, -v1, -v2); }

		inline V3V3V3 operator + (float scalar) { return V3V3V3(v0 + scalar, v1 + scalar, v2 + scalar); }
		inline V3V3V3 operator - (float scalar) { return V3V3V3(v0 - scalar, v1 - scalar, v2 - scalar); }
		inline V3V3V3 operator * (float scalar) { return V3V3V3(v0 * scalar, v1 * scalar, v2 * scalar); }
		inline V3V3V3 operator / (float scalar) { return V3V3V3(v0 / scalar, v1 / scalar, v2 / scalar); }

		inline void operator += (float scalar) { v0 += scalar; v1 += scalar; v2 += scalar; }
		inline void operator -= (float scalar) { v0 -= scalar; v1 -= scalar; v2 -= scalar; }
		inline void operator *= (float scalar) { v0 *= scalar; v1 *= scalar; v2 *= scalar; }
		inline void operator /= (float scalar) { v0 /= scalar; v1 /= scalar; v2 /= scalar; }

		inline V3V3V3 operator + (const V3V3V3& other) { return V3V3V3(v0 + other.v0, v1 + other.v1, v2 + other.v2); }
		inline friend V3V3V3 operator + (const V3V3V3& a, const V3V3V3& b) { return V3V3V3(a.v0 + b.v0, a.v1 + b.v1, a.v2 + b.v2); }

		inline void operator += (const V3V3V3& other) { v0 += other.v0; v1 += other.v1; v2 += other.v2; }
		inline void operator -= (const V3V3V3& other) { v0 -= other.v0; v1 -= other.v1; v2 -= other.v2; }

		inline V3V3V3 operator - (const V3V3V3& other) { return V3V3V3(v0 - other.v0, v1 - other.v1, v2 - other.v2); }
		inline friend V3V3V3 operator - (const V3V3V3& a, const V3V3V3& b) { return V3V3V3(a.v0 - b.v0, a.v1 - b.v1, a.v2 - b.v2); }

		//inline friend V3V3V3 operator * (const V3V3V3& a, float scalar) { return V3V3V3(a.v0 * scalar, a.v1 * scalar, a.v2 * scalar); }
		//inline friend V3V3V3 operator * (float scalar, const V3V3V3& a) { return V3V3V3(a.v0 * scalar, a.v1 * scalar, a.v2 * scalar); }

		//inline friend V3V3V3 operator / (const V3V3V3& a, float scalar) { return V3V3V3(a.v0 / scalar, a.v1 / scalar, a.v2 / scalar); }
		//inline friend V3V3V3 operator / (float scalar, const V3V3V3& a) { return V3V3V3(a.v0 / scalar, a.v1 / scalar, a.v2 / scalar); }

		inline bool operator == (const V3V3V3& other) const { return v0 == other.v0 && v1 == other.v1 && v2 == other.v2; }
		inline bool operator != (const V3V3V3& other) const { return v0 != other.v0 || v1 != other.v1 || v2 != other.v2; }

		inline bool operator < (const V3V3V3& other)
		{
			if (v0 < other.v0) return true;
			else if (v0 > other.v0) return false;
			else if (v1 < other.v1) return true;
			else if (v1 > other.v1) return false;
			else return (v2 < other.v2);
		}

		inline friend bool operator < (const V3V3V3& a, const V3V3V3& b)
		{
			if (a.v0 < b.v0) return true;
			else if (a.v0 > b.v0) return false;
			else if (a.v1 < b.v1) return true;
			else if (a.v1 > b.v1) return false;
			else return (a.v2 < b.v2);
		}

		inline bool operator >(const V3V3V3& other)
		{
			if (v0 > other.v0) return true;
			else if (v0 < other.v0) return false;
			else if (v1 > other.v1) return true;
			else if (v1 < other.v1) return false;
			else return (v2 > other.v2);
		}

		inline friend bool operator > (const V3V3V3& a, const V3V3V3& b)
		{
			if (a.v0 > b.v0) return true;
			else if (a.v0 < b.v0) return false;
			else if (a.v1 > b.v1) return true;
			else if (a.v1 < b.v1) return false;
			else return (a.v2 > b.v2);
		}
	};

	struct AABB
	{
		V3 min = V3(FLT_MAX, FLT_MAX, FLT_MAX);
		V3 max = V3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		V3 center;
		float diagonalLength = 0.0f;

		AABB() {}

		AABB(const AABB& other)
		{
			min = other.min;
			max = other.max;

			center = min + (max - min) * 0.5f;
			diagonalLength = V3::Length(min, max);
		}

		AABB(const V3& a, const V3& b, const V3& c)
		{
			MIN_V3(min, a);
			MIN_V3(min, b);
			MIN_V3(min, c);
			MAX_V3(max, a);
			MAX_V3(max, b);
			MAX_V3(max, c);

			center = min + (max - min) * 0.5f;
			diagonalLength = V3::Length(min, max);
		}

		void Insert(const V3& v)
		{
			MIN_V3(min, v);
			MAX_V3(max, v);

			center = min + (max - min) * 0.5f;
			diagonalLength = V3::Length(min, max);
		}

		void Insert(const V3& a, const V3& b, const V3& c)
		{
			MIN_V3(min, a);
			MIN_V3(min, b);
			MIN_V3(min, c);
			MAX_V3(max, a);
			MAX_V3(max, b);
			MAX_V3(max, c);

			center = min + (max - min) * 0.5f;
			diagonalLength = V3::Length(min, max);
		}

		void Insert(const AABB& other)
		{
			MIN_V3(min, other.min);
			MAX_V3(max, other.max);

			center = min + (max - min) * 0.5f;
			diagonalLength = V3::Length(min, max);
		}

		bool Contains(const AABB& other) const
		{
			return (min.x <= other.min.x && min.y <= other.min.y && min.z <= other.min.z) &&
				(max.x >= other.max.x && max.y >= other.max.y && max.z >= other.max.z);
		}

		bool Contains(const V3& p) const
		{
			return CONTAINS_V3(min, max, p);
		}

		bool Intersects(const AABB& other) const
		{
			if (min.x >= other.max.x) return false;
			if (max.x <= other.min.x) return false;
			if (min.y >= other.max.y) return false;
			if (max.y <= other.min.y) return false;
			if (min.z >= other.max.z) return false;
			if (max.z <= other.min.z) return false;
			return true;
		}

		bool IntersectsContaingVertex(const AABB& other) const
		{
			if (min.x > other.max.x) return false;
			if (max.x < other.min.x) return false;
			if (min.y > other.max.y) return false;
			if (max.y < other.min.y) return false;
			if (min.z > other.max.z) return false;
			if (max.z < other.min.z) return false;
			return true;
		}
	};
}