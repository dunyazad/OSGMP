#pragma once

#include <vector>
using namespace std;

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