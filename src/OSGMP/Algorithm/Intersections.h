#pragma once

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
