#include "RayTracerRender.h"
#define L_INTENSITY 0.5f
#define R_LENGTH 100000000.0f


//Ctor Sets up a basic scene
CRayTracer::CRayTracer(void)
{
	Scene.lights.push_back(CLight(Vec3(0, -1, 1), Vec3(100, 100, 100)));
	//Scene.lights.push_back(CLight(Vec3(0, 1, 1), Vec3(100, 100, 100)));

	Scene.spheres.push_back(CSphere(Vec3(1, 0, 10), 1.0f, Vec3(0, 0, 255)));

	Scene.triangles.push_back(CTriangle(Vec3(-1, 3, 10), Vec3(4, 3, 15), Vec3(-1, 0, 10), Vec3(0, 255, 0)));

	Scene.planes.push_back(CPlane(Vec3(0 , 0, 30), Vec3(0 , 1, -1), Vec3(255,0,0)));
}

//Renders a scene and puts result in Scene.pixels
void CRayTracer::RenderScene(void)
{
	Canvas.Clear();
	float aspectRatio = static_cast<float>(Canvas.height) / static_cast<float>(Canvas.width);

	Vec3 cameraRight = Cross(Scene.camDirection, Scene.camUp) * -aspectRatio;
	Vec3 cameraUp = Cross(cameraRight, Scene.camDirection) * aspectRatio;
	Vec3 screenCenter = Scene.camPos + Scene.camDirection;

	//Create ray for every pixel
	for (int y = 0; y < Canvas.height; y++)
	{
		for (int x = 0; x < Canvas.width; x++)
		{
			float u = ((static_cast<float>(x) / Canvas.width) - 0.5f);
			float v = ((static_cast<float>(y) / Canvas.height) - 0.5f);

			Vec3 pointOnScreen = cameraRight * u;
			pointOnScreen += cameraUp * v;
			pointOnScreen += Scene.camPos + Scene.camDirection;

			Vec3 rayDirection = pointOnScreen - Scene.camPos;
			CRay ray = CRay(Scene.camPos, rayDirection.GetNormalized(), R_LENGTH);

			int hitIndex = -1;
			CObjectTypes hitType = CObjectTypes::none;

			for (auto it = Scene.spheres.begin(); it != Scene.spheres.end(); ++it)
			{
				if (TestSphereIntersection(ray, *it) == true)
				{
					hitIndex = it - Scene.spheres.begin();
					hitType = CObjectTypes::sphere;
				}
			}
			for (auto it = Scene.triangles.begin(); it != Scene.triangles.end(); ++it)
			{
				if (TestTriangleIntersection(ray, *it) == true)
				{
					hitIndex = it - Scene.triangles.begin();
					hitType = CObjectTypes::triangle;
				}
			}
			for (auto it = Scene.planes.begin(); it != Scene.planes.end(); ++it)
			{
				if (TestPlaneIntersection(ray, *it) == true)
				{
					hitIndex = it - Scene.planes.begin();
					hitType = CObjectTypes::plane;
				}
			}

			if (hitIndex != -1)
			{
				Vec3 resultColor = Vec3(0, 0, 0);

				for (auto it = Scene.lights.begin(); it != Scene.lights.end(); ++it)
				{
					Vec3 hitPoint = ray.origin + ray.direction * ray.length;
					resultColor += ComputePixelColor(hitType, hitIndex, hitPoint, *it);
				}
				resultColor.Clamp255();
				Canvas.pixels[(y * Canvas.width) + x] = resultColor;
			}

		}
	}

}

//Clears all objects from scene
void CRayTracer::ClearScene(void)
{
	Scene.spheres.clear();
	Scene.triangles.clear();
	Scene.planes.clear();
	Scene.lights.clear();
}

//Computes the resulting color of pixel
Vec3 CRayTracer::ComputePixelColor(CObjectTypes hitType, int hitIndex, Vec3 hitPoint, CLight light)
{
	Vec3 lightRayDirection = light.position - hitPoint;
	float backLength = lightRayDirection.Len();
	lightRayDirection.Normalize();
	CRay backRay = CRay(hitPoint, lightRayDirection,backLength);

	Vec3 normal = Vec3(0, 0, 0);
	Vec3 color = Vec3(0, 0, 0);
	

	if (IsOccluded(backRay, hitType, hitIndex)) return color;
	

	if (hitType == CObjectTypes::sphere)
	{
		normal = Scene.spheres[hitIndex].GetNormal(hitPoint);
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += Scene.spheres[hitIndex].color * d;
		color += light.color * d * L_INTENSITY;
	}
	else if (hitType == CObjectTypes::triangle)
	{
		normal = Scene.triangles[hitIndex].GetNormal();
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += Scene.triangles[hitIndex].color * d;
		color += light.color * d * L_INTENSITY;
	}
	else if (hitType == CObjectTypes::plane)
	{
		normal = Scene.planes[hitIndex].normal;
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += Scene.planes[hitIndex].color * d;
		color += light.color * d * L_INTENSITY;
	}

	return color;
}

//Tests if the ray hits the sphere
bool CRayTracer::TestSphereIntersection(CRay& ray, CSphere& sphere)
{
	Vec3 originToSphere = sphere.center - ray.origin;
	float projection = Dot(originToSphere, ray.direction);
	Vec3 distanceVector = originToSphere - ray.direction * projection;
	float distanceSq = Dot(distanceVector, distanceVector);
	float radiusSq = sphere.radius * sphere.radius;

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

//Tests if the ray hits the triangle
bool CRayTracer::TestTriangleIntersection(CRay& ray, CTriangle& triangle)
{
	Vec3 p0p1 = triangle.p1 - triangle.p0;
	Vec3 p0p2 = triangle.p2 - triangle.p0;
	Vec3 pvec = Cross(ray.direction,p0p2);
	float det = Dot(pvec, p0p1);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < EPSILON) return false;
	
	float invDet = 1 / det;

	Vec3 tvec = ray.origin - triangle.p0;
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

//Test if the ray hits the plane
bool CRayTracer::TestPlaneIntersection(CRay& ray, CPlane& plane)
{
	float det = Dot(plane.normal, ray.direction);

	if (fabs(det) < EPSILON) return false;

	Vec3 distance = plane.point - ray.origin;
	float newLength = Dot(distance, plane.normal) / det;

	if (newLength < 0.0f) return false;

	if (newLength < ray.length)
	{
		ray.length = newLength;
		return true;
	}

	return false;
}

//Tests if the hitted point is occluded by another object (shadow casting)
bool CRayTracer::IsOccluded(CRay& ray, CObjectTypes hitType, int hitIndex)
{
	for (auto it = Scene.spheres.begin(); it != Scene.spheres.end(); ++it)
	{
		if ((hitType == CObjectTypes::sphere) && (hitIndex == (it - Scene.spheres.begin()))) return false;

		if (TestSphereIntersection(ray, *it) == true)
		{
			return true;
		}
	}
	for (auto it = Scene.triangles.begin(); it != Scene.triangles.end(); ++it)
	{
		if ((hitType == CObjectTypes::triangle) && (hitIndex == (it - Scene.triangles.begin()))) return false;

		if (TestTriangleIntersection(ray, *it) == true)
		{
			return true;
		}
	}
	for (auto it = Scene.planes.begin(); it != Scene.planes.end(); ++it)
	{
		if ((hitType == CObjectTypes::plane) && (hitIndex == (it - Scene.planes.begin()))) return false;

		if (TestPlaneIntersection(ray, *it) == true)
		{
			return true;
		}
	}
	return false;
}

