#pragma once
#include "MyMath.h"
#include <vector>
#include <memory>
#include <utility>

enum class CObjectTypes
{
	none,
	sphere,
	triangle,
	plane
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

class CBasicObject
{
public:
	CBasicObject(Vec3 color) : color(color) {};
	virtual ~CBasicObject() {};

	virtual bool TestIntersection(CRay& ray) = 0;
	virtual Vec3 GetNormal(Vec3 point) = 0;

	Vec3 color;
	CObjectTypes type = CObjectTypes::none;
};

class CSphere : public CBasicObject
{
public:
	CSphere(Vec3 center, float radius, Vec3 color) :
		CBasicObject(color)
	{
		type = CObjectTypes::sphere;
		this->center = center;
		this->radius = radius;
	}

	Vec3 center;
	float radius;

	bool TestIntersection(CRay& ray);

	Vec3 GetNormal(Vec3 hitPoint)
	{
		Vec3 normal = hitPoint - center;
		return normal;
	};
};

class CTriangle : public CBasicObject
{
public:
	CTriangle(Vec3 p0, Vec3 p1, Vec3 p2, Vec3(color)) :
		CBasicObject(color),
		p0(p0),
		p1(p1),
		p2(p2)
	{
		this->color = color;
		type = CObjectTypes::triangle;
	}

	Vec3 p0;
	Vec3 p1;
	Vec3 p2;

	bool TestIntersection(CRay& ray);

	Vec3 GetNormal(Vec3 hitPoint)
	{
		Vec3 v0v1 = p1 - p0;
		Vec3 v0v2 = p2 - p0;
		Vec3 normal = Cross(v0v1, v0v2);
		return normal;
	};
};

class CPlane : public CBasicObject
{
public:
	CPlane(Vec3 point, Vec3 normal, Vec3 color) :
		CBasicObject(color),
		point(point),
		normal(normal)
	{
		this->color = color;
		type = CObjectTypes::plane;
	}

	Vec3 point;
	Vec3 normal;

	bool TestIntersection(CRay& ray);

	Vec3 GetNormal(Vec3 hitPoint)
	{
		return normal;
	};
};

class CScene
{
public:
	CScene& operator=(const CScene&) = delete;
	CScene(const CScene&) = delete;
	CScene() = default;

	Vec3 camPos = Vec3(0, 0, 0);
	Vec3 camDirection = Vec3(0, 0, 1);
	Vec3 camUp = Vec3(0 , 1 , 0);

	std::vector<CLight> lights;
	std::vector<std::unique_ptr<CBasicObject>> objects;
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
