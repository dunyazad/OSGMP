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
