#pragma once
#include "framework/pch.h"
#include "vector3f.h"

struct Quaternion
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	static Quaternion getIdentity()
	{
		Quaternion q;
		q.w = 1.0f;
		return q;
	}
	static Quaternion normalize(Quaternion q)
	{
		float n = 1.0f / sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
		q.x *= n;
		q.y *= n;
		q.z *= n;
		q.w *= n;
		return q;
	}
	static Quaternion getRotationQuaterion(Vector3f axis, float angle)
	{
		Quaternion q = Quaternion::getIdentity();
		q.x = axis.x * sin(toRadians(angle) / 2);
		q.y = axis.y * sin(toRadians(angle) / 2);
		q.z = axis.z * sin(toRadians(angle) / 2);
		q.w = cos(toRadians(angle) / 2);
		return q;
	}
	Quaternion operator*(const Quaternion& m)
	{
		//THIS-> is quat 1
		Vector3f v1;
		v1.x = this->x;
		v1.y = this->y;
		v1.z = this->z;
		Vector3f v2;
		v2.x = m.x;
		v2.y = m.y;
		v2.z = m.z;

		float w = (this->w * m.w) - Vector3f::dot(v1, v2);

		Vector3f vr1 = v2 * this->w;
		Vector3f vr2 = v1 * m.w;
		Vector3f vr3 = Vector3f::cross(v2, v1);
		Vector3f r = vr1 + vr2 + vr3;

		Quaternion result;
		result.x = r.x;
		result.y = r.y;
		result.z = r.z;
		result.w = w;

		return result;
	}
};