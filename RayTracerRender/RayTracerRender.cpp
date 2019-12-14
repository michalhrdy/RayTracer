#include "RayTracerRender.h"
#define L_INTENSITY 0.5f
#define R_LENGTH 1000000.0f


//Ctor Sets up a basic scene
CRayTracer::CRayTracer(void)
{
	Scene.lights.push_back(CLight(Vec3(0, -1, 1), Vec3(100, 100, 100)));

	Scene.objects.push_back(std::make_unique<CSphere>(Vec3(1, 0, 10), 1.0f, Vec3(0, 0, 255)));

	Scene.objects.push_back(std::make_unique<CTriangle>(Vec3(-1, 3, 10), Vec3(4, 3, 15), Vec3(-1, 0, 10), Vec3(0, 255, 0)));
	
	Scene.objects.push_back(std::make_unique<CPlane>(Vec3(0, 0, 30), Vec3(0, 1, -1), Vec3(255, 0, 0)));
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

			bool rayHit = false;

			for (auto it = Scene.objects.begin(); it != Scene.objects.end(); ++it)
			{
				bool rayHit = false;

				if (it->get()->type == CObjectTypes::sphere)
				{
					CSphere sphere = dynamic_cast<CSphere&>(*it->get());
					rayHit = sphere.TestIntersection(ray);
				}
				else if (it->get()->type == CObjectTypes::triangle)
				{
					CTriangle triangle = dynamic_cast<CTriangle&>(*it->get());
					rayHit = triangle.TestIntersection(ray);
				}
				else if (it->get()->type == CObjectTypes::plane)
				{
					CPlane plane = dynamic_cast<CPlane&>(*it->get());
					rayHit = plane.TestIntersection(ray);
				}

				if (rayHit == true)
				{
					hitIndex = it - Scene.objects.begin();
					hitType = it->get()->type;
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
	Scene.objects.clear();
	Scene.lights.clear();
}

//Computes the resulting color of pixel
Vec3 CRayTracer::ComputePixelColor(CObjectTypes hitType, int hitIndex, Vec3 hitPoint, CLight light)
{
	Vec3 lightRayDirection = light.position - hitPoint;
	float backLength = lightRayDirection.Len();
	lightRayDirection.Normalize();
	CRay backRay = CRay(hitPoint, lightRayDirection, backLength);

	Vec3 normal = Vec3(0, 0, 0);
	Vec3 color = Vec3(0, 0, 0);

	//Shadow casting
	//***************
	if (IsOccluded(backRay, hitIndex)) return color;
	//***************

	if (hitType == CObjectTypes::sphere)
	{
		CSphere sphere = dynamic_cast<CSphere&>(*Scene.objects[hitIndex]);

		normal = sphere.GetNormal(hitPoint);
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += sphere.color * d;
		color += light.color * d * L_INTENSITY;
	}
	else if (hitType == CObjectTypes::triangle)
	{
		CTriangle triangle = dynamic_cast<CTriangle&>(*Scene.objects[hitIndex]);

		normal = triangle.GetNormal(Vec3(0,0,0));
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += triangle.color * d;
		color += light.color * d * L_INTENSITY;
	}
	else if (hitType == CObjectTypes::plane)
	{
		CPlane plane = dynamic_cast<CPlane&>(*Scene.objects[hitIndex]);

		normal = plane.GetNormal(Vec3(0, 0, 0));
		normal.Normalize();
		float d = Dot(normal, lightRayDirection);
		if (d < 0.0f) d = 0.0f;

		color += plane.color * d;
		color += light.color * d * L_INTENSITY;
	}
	return color;
}

//Tests if the hitted point is occluded by another object (shadow casting)
bool CRayTracer::IsOccluded(CRay& ray, int hitIndex)
{
	for (auto it = Scene.objects.begin(); it != Scene.objects.end(); ++it)
	{
		if (hitIndex == (it - Scene.objects.begin())) return false;

		if (it->get()->type == CObjectTypes::sphere)
		{
			CSphere sphere = dynamic_cast<CSphere&>(*it->get());
			if (sphere.TestIntersection(ray) == true)
			{
				return true;
			}
		}
		else if (it->get()->type == CObjectTypes::triangle)
		{
			CTriangle triangle = dynamic_cast<CTriangle&>(*it->get());
			if (triangle.TestIntersection(ray) == true)
			{
				return true;
			}
		}
		else if (it->get()->type == CObjectTypes::plane)
		{
			CPlane plane = dynamic_cast<CPlane&>(*it->get());
			if (plane.TestIntersection(ray) == true)
			{
				return true;
			}
		}
	}
	return false;
}

