#pragma once
#include "SceneObjects.h"

bool CSphere::TestIntersection(CRay& ray)
{
	Vec3 originToSphere = center - ray.origin;
	float projection = Dot(originToSphere, ray.direction);
	Vec3 distanceVector = originToSphere - ray.direction * projection;
	float distanceSq = Dot(distanceVector, distanceVector);
	float radiusSq = radius * radius;

	//compare distance of the closest point on ray with radius of sphere
	if (distanceSq > radiusSq) return false;

	float newLength = projection - sqrtf(radiusSq - distanceSq);
	if ((newLength < ray.length) && (newLength > 0.0f))
	{
		ray.length = newLength;
		return true;
	}
	return false;
}

bool CTriangle::TestIntersection(CRay& ray)
{
	Vec3 p0p1 = p1 - p0;
	Vec3 p0p2 = p2 - p0;
	Vec3 pvec = Cross(ray.direction, p0p2);
	float det = Dot(pvec, p0p1);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < EPSILON) return false;

	float invDet = 1 / det;

	Vec3 tvec = ray.origin - p0;
	float u = Dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	Vec3 qvec = Cross(tvec, p0p1);
	float v = Dot(ray.direction, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	float newLength = Dot(qvec, p0p2) * invDet;

	if (newLength < ray.length && (newLength > 0.0f))
	{
		ray.length = newLength;
		return true;
	}

	return false;
}

bool CPlane::TestIntersection(CRay& ray)
{
	float det = Dot(normal, ray.direction);

	if (fabs(det) < EPSILON) return false;

	Vec3 distance = point - ray.origin;
	float newLength = Dot(distance, normal) / det;

	if (newLength < 0.0f) return false;

	if (newLength < ray.length)
	{
		ray.length = newLength;
		return true;
	}

	return false;
}

void CCanvas::Clear(void)
{
	pixels.clear();
	pixels.resize(width * height);
}