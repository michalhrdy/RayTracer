#pragma once
#include "MyMath.h"
#include <vector>


enum class CObjectTypes
{
	none,
	sphere,
	triangle,
	plane
};

class CLight
{
public:
	CLight(Vec3 position, Vec3 color) :
		position(position),
		color(color)
	{}

	Vec3 position;
	Vec3 color;
};

class CSphere
{
public:
	CSphere(Vec3 center, float radius, Vec3 color) :
		center(center),
		radius(radius),
		color(color)
	{}

	Vec3 center;
	float radius;
	Vec3 color;

	Vec3 GetNormal(Vec3 hitPoint)
	{
		Vec3 normal = hitPoint - center;
		return normal;
	};
};

class CTriangle
{
public:
	CTriangle(Vec3 p0, Vec3 p1, Vec3 p2, Vec3(color)) :
		p0(p0),
		p1(p1),
		p2(p2),
		color(color)
	{}

	Vec3 p0;
	Vec3 p1;
	Vec3 p2;
	Vec3 color;

	Vec3 GetNormal(void)
	{
		Vec3 v0v1 = p1 - p0;
		Vec3 v0v2 = p2 - p0;
		Vec3 normal = Cross(v0v1, v0v2);
		return normal;
	};
};

class CPlane
{
public:
	CPlane(Vec3 point, Vec3 normal, Vec3 color) :
		point(point),
		normal(normal),
		color(color)
	{}

	Vec3 point;
	Vec3 normal;
	Vec3 color;
};

class CScene
{
public:
	Vec3 camPos = Vec3(0, 0, 0);
	Vec3 camDirection = Vec3(0, 0, 1);
	Vec3 camUp = Vec3(0 , 1 , 0);

	std::vector<CLight> lights;
	std::vector<CSphere> spheres;
	std::vector<CTriangle> triangles;
	std::vector<CPlane>	planes;
};

class CCanvas
{
public:
	CCanvas(void) :
		width(800),
		height(800)
	{
		pixels.resize(width*height);
	}
	CCanvas(int width, int height) :
		width(width),
		height(height)
	{
		pixels.resize(width*height);
	}

	int width;
	int height;
	std::vector<Vec3> pixels;

	void Clear(void);
};

class CRay
{
public:
	CRay(Vec3 origin, Vec3 direction, float length) :
		origin(origin),
		direction(direction),
		length(length)
	{}

	Vec3 origin, direction;
	float length;
};