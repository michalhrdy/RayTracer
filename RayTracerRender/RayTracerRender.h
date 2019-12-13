#pragma once
#include "MyMath.h"
#include "SceneObjects.h"


#ifdef RENDERDLL
#define DLLDIR  __declspec(dllexport)   // export DLL information

#else
#define DLLDIR  __declspec(dllimport)   // import DLL information

#endif 

//Class that provides interface to set up a scene and render an image
class DLLDIR CRayTracer
{
public:
	CRayTracer(void);

	int GetCanvasWidth(void) { return Canvas.width; }
	int GetCanvasHeight(void) { return Canvas.height; }
	std::vector<Vec3> GetResultImage(void) { return Canvas.pixels; }
	Vec3 GetPixelColor(int x, int y) { return Canvas.pixels[(y * Canvas.width) + x]; }


	void SetCanvasSize(int width, int height) { Canvas.width = width; Canvas.height = height; Canvas.pixels.resize(width * height); }
	void SetCameraPosition(Vec3 position) { Scene.camPos = position; }
	void SetCameraUp(Vec3 up) { Scene.camUp = up; }
	void SetCameraDirection(Vec3 direction) { Scene.camDirection = direction; }

	void AddLight(Vec3 position, Vec3 color) { Scene.lights.push_back(CLight(position, color));}
	void AddSphere(Vec3 center, float radius, Vec3 color) { Scene.spheres.push_back(CSphere(center, radius, color)); }
	void AddTriangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 color) { Scene.triangles.push_back(CTriangle(p1, p2, p3, color)); }
	void AddPlane(Vec3 point, Vec3 normal, Vec3 color) { Scene.planes.push_back(CPlane(point, normal, color)); }

	void RenderScene(void);
	void ClearScene(void);

private:
	CCanvas Canvas;
	CScene Scene;

	Vec3 ComputePixelColor(CObjectTypes hitType, int index, Vec3 hitPoint, CLight light);

	bool TestSphereIntersection(CRay& ray, CSphere& sphere);
	bool TestTriangleIntersection(CRay& ray, CTriangle& triangle);
	bool TestPlaneIntersection(CRay& ray, CPlane& plane);
	bool IsOccluded(CRay& ray, CObjectTypes hitType, int hitIndex);
};

