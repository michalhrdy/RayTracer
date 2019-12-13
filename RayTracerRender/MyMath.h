#pragma once
#include <cmath>
#define EPSILON 1e-4f

template <typename T>
class _Vec3
{
public:
	_Vec3()
		:
		x((T)0),
		y((T)0),
		z((T)0)
	{}
	_Vec3(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}
	_Vec3(const _Vec3& vect)
		:
		_Vec3(vect.x, vect.y, vect.z)
	{}

	template <typename T2>
	explicit operator _Vec3<T2>() const
	{
		return{ (T2)x,(T2)y,(T2)z };
	}

	T		LenSq() const
	{
		return (*this) * (*this);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	_Vec3& Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	_Vec3	GetNormalized() const
	{
		_Vec3 norm = *this;
		norm.Normalize();
		return norm;
	}
	_Vec3& Clamp255()
	{
		x = (x > 255) ? 255 : (x < 0) ? 0 : x;
		y = (y > 255) ? 255 : (y < 0) ? 0 : y;
		z = (z > 255) ? 255 : (z < 0) ? 0 : z;
		return *this;
	}
	_Vec3	operator-() const
	{
		return _Vec3(-x, -y, -z);
	}
	_Vec3& operator=(const _Vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	_Vec3& operator+=(const _Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	_Vec3& operator-=(const _Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	T		operator*(const _Vec3& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	_Vec3	operator+(const _Vec3& rhs) const
	{
		return _Vec3(*this) += rhs;
	}
	_Vec3	operator-(const _Vec3& rhs) const
	{
		return _Vec3(*this) -= rhs;
	}
	_Vec3& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	_Vec3	operator*(const T& rhs) const
	{
		return _Vec3(*this) *= rhs;
	}
	_Vec3& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	_Vec3	operator/(const T& rhs) const
	{
		return _Vec3(*this) /= rhs;
	}
	bool	operator==(const _Vec3& rhs) const
	{
		return x == rhs.x && y == rhs.y && rhs.z = z;
	}
	bool	operator!=(const _Vec3& rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x, y, z;
};

template <typename T>
static float Dot(const _Vec3<T>& lhs, const _Vec3<T>& rhs)
{
	return lhs * rhs;
}

template <typename T>
static _Vec3<T> Cross(const _Vec3<T>& lhs, const _Vec3<T>& rhs)
{
	return _Vec3<T>(lhs.y * rhs.z - rhs.y * lhs.z,
		lhs.z * rhs.x - rhs.z * lhs.x,
		lhs.x * rhs.y - rhs.x * lhs.y);
}

typedef _Vec3<float> Vec3;
typedef _Vec3<double> Ved3;
typedef _Vec3<int> Vei3;