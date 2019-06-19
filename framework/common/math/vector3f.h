#pragma once
#include "framework/pch.h"
#include "mathfuncs.h"

struct Vector3f
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3f() {}
	Vector3f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	static Vector3f getx()
	{
		Vector3f v;
		v.x = 1.0f;
		return v;
	}
	static Vector3f gety()
	{
		Vector3f v;
		v.y = 1.0f;
		return v;
	}
	static Vector3f getz()
	{
		Vector3f v;
		v.z = 1.0f;
		return v;
	}
	static Vector3f getForward(Vector3f rotation)
	{
		Vector3f direction;
		direction.z = cos(toRadians(rotation.x)) * cos(toRadians(rotation.y));
		direction.y = -sin(toRadians(rotation.x));
		direction.x = cos(toRadians(rotation.x)) * sin(toRadians(rotation.y));
		return direction;
	}
	static Vector3f getRight(Vector3f up, Vector3f direction)
	{
		Vector3f result = Vector3f::cross(up, Vector3f::normalize(direction));
		return result;
	}
	static Vector3f normalize(Vector3f vector)
	{
		float length = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;
		return vector;
	}
	static float dot(Vector3f v1, Vector3f v2)
	{
		float r = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
		return r;
	}
	static Vector3f cross(Vector3f v1, Vector3f v2)
	{
		Vector3f r;
		r.x = (v1.y * v2.z) - (v1.z * v2.y);
		r.y = (v1.z * v2.x) - (v1.x * v2.z);
		r.z = (v1.x * v2.y) - (v1.y * v2.x);
		return r;
	}
	Vector3f operator*(const Vector3f& v)
	{
		Vector3f r;
		r.x = (this->y * v.z) - (this->z * v.y);
		r.y = (this->z * v.x) - (this->x * v.z);
		r.z = (this->x * v.y) - (this->y * v.x);
		return r;
	}
	Vector3f operator*(const float& s)
	{
		Vector3f r;
		r.x = this->x * s;
		r.y = this->y * s;
		r.z = this->z * s;
		return r;
	}
	Vector3f operator+(const Vector3f& v)
	{
		Vector3f r;
		r.x = this->x + v.x;
		r.y = this->y + v.y;
		r.z = this->z + v.z;
		return r;
	}
	Vector3f operator-(const Vector3f& v)
	{
		Vector3f r;
		r.x = this->x - v.x;
		r.y = this->y - v.y;
		r.z = this->z - v.z;
		return r;
	}
};
