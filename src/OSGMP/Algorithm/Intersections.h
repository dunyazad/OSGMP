#pragma once

#include <vector>
using namespace std;

inline bool FloatEqual(float a, float b)
{
	float epsilon;
	/* May as well do the easy check first. */
	if (a == b)
		return true;

	if (a > b) {
		epsilon = a * 0.000001f;
	}
	else {
		epsilon = b * 0.000001f;
	}

	return fabs(a - b) < epsilon;
}

template<typename T>
inline bool IsPointOnTriangle(const T& p, const T& v0, const T& v1, const T& v2)
{
	auto u = v1 - v0;
	auto v = v2 - v0;
	auto n = u ^ v;
	auto w = p - v0;
	auto gamma = ((u ^ w) * n) / (n * n);
	auto beta = ((w ^ v) * n) / (n * n);
	auto alpha = 1 - gamma - beta;
	return ((0 <= alpha) && (alpha <= 1) &&
		(0 <= beta) && (beta <= 1) &&
		(0 <= gamma) && (gamma <= 1));
}

template<typename T>
bool IntersectRayPlane(const T& rayOrigin, const T& rayDir, const T& planeOrigin, const T& planeNormal, T& intersection)
{
	float d = rayDir * planeNormal;
	if (d == 0) return false;
	else if (d < -1e-6) {
		float distance = ((planeOrigin - rayOrigin) * planeNormal) / d;
		intersection = rayOrigin + rayDir * distance;
		return true;
	}

	return false;
}

template<typename T>
bool IntersectRayRay(const T& ao, const T& ad, const T& bo, const T& bd, T& point)
{
	// find a = v x u
	auto a = ad ^ bd;            // cross product

	// if v and u are parallel, then no intersection, return NaN point
	if (FloatEqual(a.x(), 0) && FloatEqual(a.y(), 0) && FloatEqual(a.z(), 0))
		//return Vec3(NAN, NAN, NAN);
		return false;

	// find b = (Q1-P1) x u
	auto b = (bo - ao) ^ bd;      // cross product

	// find t = b/a = (Q1-P1) x u / (v x u)
	float t = 0;
	if (FloatEqual(a.x(), 0) == false)
		t = b.x() / a.x();
	else if (FloatEqual(a.y(), 0) == false)
		t = b.y() / a.y();
	else if (FloatEqual(a.z(), 0) == false)
		t = b.z() / a.z();

	// find intersection point
	point = ao + (ad * t);       // substitute t to line1
	return true;
}

template<typename T>
bool PointOnLine(const T& p, const T& la, const T& lb, bool includeVertex)
{
	if (includeVertex)
	{
		if (p == la || p == lb)
		{
			return true;
		}
	}
	else
	{
		if (p == la || p == lb)
		{
			return false;
		}
	}

	auto d = lb - la;

	auto t = (d * (p - la)) / (d * d);
	if (includeVertex)
	{
		return t >= 0 && t <= 1.0f;
	}
	else
	{
		return t > 0 && t < 1.0f;
	}
}

template <typename T>
bool PointOnTriangle(const T& p, const T& a, const T& b, const T& c, bool bIncludeEdge)
{
	auto v0 = c - a;
	auto v1 = b - a;
	auto v2 = p - a;

	auto dot00 = v0 * v0;
	auto dot01 = v0 * v1;
	auto dot02 = v0 * v2;
	auto dot11 = v1 * v1;
	auto dot12 = v1 * v2;

	auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	if (bIncludeEdge)
	{
		return (u >= 0) && (v >= 0) && (u + v <= 1);
	}
	else
	{
		return (u > 0) && (v > 0) && (u + v < 1);
	}
}

template<typename T>
bool EnableToTriangle(const T& p0, const T& p1, const T& p2)
{
	auto d01 = p1 - p0;
	auto d02 = p2 - p0;
	return (d01 * d02) != 0;
}

template<typename T>
bool IntersectLineLine(const T& lava, const T& lavb, const T& lbva, const T& lbvb, T& intersection, bool bIncludeVertex)
{
	auto rao = lava;
	auto rad = lavb - lava;
	rad.normalize();

	auto rbo = lbva;
	auto rbd = lbvb - lbva;
	rbd.normalize();

	if (lava == lbva || lavb == lbvb || lavb == lbva || lavb == lbvb)
	{
		if (bIncludeVertex)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (IntersectRayRay<T>(rao, rad, rbo, rbd, intersection))
	{
		if (intersection == lava || intersection == lavb || intersection == lbva || intersection == lbvb)
		{
			if (bIncludeVertex)
			{
				if (PointOnLine<T>(intersection, lava, lavb, bIncludeVertex) && PointOnLine<T>(intersection, lbva, lbvb, bIncludeVertex))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	if (PointOnLine<T>(intersection, lava, lavb, bIncludeVertex) && PointOnLine<T>(intersection, lbva, lbvb, bIncludeVertex))
	{
		return true;
	}

	return false;
}

template<typename T>
void ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint)
{
	IntersectRayPlane<T>(inputPoint, -projectionPlaneNormal, projectionPlanePosition, projectionPlaneNormal, projectedPoint);
}

template<typename T>
void ProjectToPlane(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints)
{
	for (auto& ip : inputPoints)
	{
		T rp;
		if (IntersectRayPlane<T>(ip, -projectionPlaneNormal, projectionPlanePosition, projectionPlaneNormal, rp))
		{
			projectedPoints.push_back(rp);
		}
		else
		{
			projectedPoints.push_back(ip);
		}
	}
}

template<typename T>
void ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const T& inputPoint, T& projectedPoint)
{
	float x = projectionPlaneNormal.x();
	float y = projectionPlaneNormal.y();
	float z = projectionPlaneNormal.z();
	if (x > y && x > z)
	{
		ProjectToPlane(projectionPlanePosition, T(1, 0, 0), inputPoint, projectedPoint);
	}
	else if (y > x && y > z)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 1, 0), inputPoint, projectedPoint);
	}
	else if (z > x && z > y)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 0, 1), inputPoint, projectedPoint);
	}
}

template<typename T>
void ProjectToAxis(const T& projectionPlanePosition, const T& projectionPlaneNormal, const vector<T>& inputPoints, vector<T>& projectedPoints)
{
	float x = projectionPlaneNormal.x();
	float y = projectionPlaneNormal.y();
	float z = projectionPlaneNormal.z();
	if (x > y && x > z)
	{
		ProjectToPlane(projectionPlanePosition, T(1, 0, 0), inputPoints, projectedPoints);
	}
	else if (y > x && y > z)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 1, 0), inputPoints, projectedPoints);
	}
	else if (z > x && z > y)
	{
		ProjectToPlane(projectionPlanePosition, T(0, 0, 1), inputPoints, projectedPoints);
	}
}

template<typename T>
bool Circumcircle(const T& p0, const T& p1, const T& p2, T& center, float& radius)
{
	float dA = p0.x() * p0.x() + p0.y() * p0.y();
	float dB = p1.x() * p1.x() + p1.y() * p1.y();
	float dC = p2.x() * p2.x() + p2.y() * p2.y();

	float aux1 = (dA*(p2.y() - p1.y()) + dB*(p0.y() - p2.y()) + dC*(p1.y() - p0.y()));
	float aux2 = -(dA*(p2.x() - p1.x()) + dB*(p0.x() - p2.x()) + dC*(p1.x() - p0.x()));
	float div = (2 * (p0.x()*(p2.y() - p1.y()) + p1.x()*(p0.y() - p2.y()) + p2.x()*(p1.y() - p0.y())));

	if (div == 0){
		return false;
	}

	center.x() = aux1 / div;
	center.y() = aux2 / div;

	radius = sqrtf((center.x() - p0.x())*(center.x() - p0.x()) + (center.y() - p0.y())*(center.y() - p0.y()));

	return true;
}

template<typename T>
bool InCircumcircle(const T& tv0, const T& tv1, const T& tv2, const T& p, bool isProjected)
{
	if (isProjected == false)
	{
		float dA = tv0.x() * tv0.x() + tv0.y() * tv0.y();
		float dB = tv1.x() * tv1.x() + tv1.y() * tv1.y();
		float dC = tv2.x() * tv2.x() + tv2.y() * tv2.y();

		float aux1 = (dA*(tv2.y() - tv1.y()) + dB*(tv0.y() - tv2.y()) + dC*(tv1.y() - tv0.y()));
		float aux2 = -(dA*(tv2.x() - tv1.x()) + dB*(tv0.x() - tv2.x()) + dC*(tv1.x() - tv0.x()));
		float div = (2 * (tv0.x()*(tv2.y() - tv1.y()) + tv1.x()*(tv0.y() - tv2.y()) + tv2.x()*(tv1.y() - tv0.y())));

		float centerx = aux1 / div;
		float centery = aux2 / div;

		float distanceSquared = (p.x() - centerx) * (p.x() - centerx) + (p.y() - centery) * (p.y() - centery);
		float radiusSquared = (centerx - tv0.x())*(centerx - tv0.x()) + (centery - tv0.y())*(centery - tv0.y());

		if (radiusSquared >= distanceSquared)
			return true;
		else
			return false;
	}
	else
	{
		vector<T> projected;
		auto d01 = tv1 - tv0;
		auto d02 = tv2 - tv0;
		auto normal = d01 ^ d02;
		normal.normalize();

		vector<Vec3> input;
		input.push_back(tv0);
		input.push_back(tv1);
		input.push_back(tv2);
		input.push_back(p);

		ProjectToAxis((tv0 + tv1 + tv2) / 3, normal, input, projected);

		auto& ptv0 = projected[0];
		auto& ptv1 = projected[1];
		auto& ptv2 = projected[2];
		auto& pp = projected[3];

		float dA = ptv0.x() * ptv0.x() + ptv0.y() * ptv0.y();
		float dB = ptv1.x() * ptv1.x() + ptv1.y() * ptv1.y();
		float dC = ptv2.x() * ptv2.x() + ptv2.y() * ptv2.y();

		float aux1 = (dA*(ptv2.y() - ptv1.y()) + dB*(ptv0.y() - ptv2.y()) + dC*(ptv1.y() - ptv0.y()));
		float aux2 = -(dA*(ptv2.x() - ptv1.x()) + dB*(ptv0.x() - ptv2.x()) + dC*(ptv1.x() - ptv0.x()));
		float div = (2 * (ptv0.x()*(ptv2.y() - ptv1.y()) + ptv1.x()*(ptv0.y() - ptv2.y()) + ptv2.x()*(ptv1.y() - ptv0.y())));

		float centerx = aux1 / div;
		float centery = aux2 / div;

		float distanceSquared = (pp.x() - centerx) * (pp.x() - centerx) + (pp.y() - centery) * (pp.y() - centery);
		float radiusSquared = (centerx - ptv0.x())*(centerx - ptv0.x()) + (centery - ptv0.y())*(centery - ptv0.y());

		if (radiusSquared >= distanceSquared)
			return true;
		else
			return false;
	}
}