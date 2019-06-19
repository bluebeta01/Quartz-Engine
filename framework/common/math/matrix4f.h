#pragma once
#include "framework/pch.h"
#include "mathfuncs.h"
#include "vector3f.h"
#include "quaternion.h"
#include "transform.h"

struct Matrix4f
{
	float values[16] = { 0 };

	static Matrix4f getIdentity()
	{
		Matrix4f mat;
		mat.values[0] = 1.0f;
		mat.values[5] = 1.0f;
		mat.values[10] = 1.0f;
		mat.values[15] = 1.0f;
		return mat;
	}
	static Matrix4f getPerspective(float aspect, float fov, float nearz, float farz)
	{
		Matrix4f mat;

		/*fov = fov * (3.14159f / 180.0f);
		float f = 1.0f / tan(fov / 2.0f);

		mat.values[0] = f / aspect;
		mat.values[5] = f;
		mat.values[10] = (far + near) / (near - far);
		mat.values[11] = -1.0f;
		mat.values[14] = (2.0f * far * near) / (near - far);*/

		float zRange = nearz - farz;
		float tanHalfFov = tanf(toRadians(fov / 2.0f));

		mat.values[0] = 1.0f / (tanHalfFov * aspect);
		mat.values[5] = 1.0f / tanHalfFov;
		mat.values[10] = (-nearz - farz) / zRange;
		mat.values[11] = 1.0f;
		mat.values[14] = 2.0f * farz * nearz / zRange;

		return mat;
	}
	static Matrix4f getViewMatrix(Transform transform)
	{
		transform.rotation.x = toRadians(transform.rotation.x);
		transform.rotation.y = toRadians(transform.rotation.y);
		transform.rotation.z = toRadians(transform.rotation.z);

		float cosPitch = cos(transform.rotation.x);
		float sinPitch = sin(transform.rotation.x);
		float  cosYaw = cos(transform.rotation.y);
		float sinYaw = sin(transform.rotation.y);

		Vector3f xaxis;
		xaxis.x = cosYaw;
		xaxis.y = 0;
		xaxis.z = -sinYaw;
		Vector3f yaxis;
		yaxis.x = sinYaw * sinPitch;
		yaxis.y = cosPitch;
		yaxis.z = cosYaw * sinPitch;
		Vector3f zaxis;
		zaxis.x = sinYaw * cosPitch;
		zaxis.y = -sinPitch;
		zaxis.z = cosPitch * cosYaw;

		Matrix4f view;

		view.values[0] = xaxis.x;
		view.values[1] = yaxis.x;
		view.values[2] = zaxis.x;
		view.values[3] = 0;

		view.values[4] = xaxis.y;
		view.values[5] = yaxis.y;
		view.values[6] = zaxis.y;
		view.values[7] = 0;

		view.values[8] = xaxis.z;
		view.values[9] = yaxis.z;
		view.values[10] = zaxis.z;
		view.values[11] = 0;

		view.values[12] = -Vector3f::dot(xaxis, transform.position);
		view.values[13] = -Vector3f::dot(yaxis, transform.position);
		view.values[14] = -Vector3f::dot(zaxis, transform.position);
		view.values[15] = 1.0f;

		Matrix4f invert = Matrix4f::getIdentity();
		invert.values[10] = -1.0f;
		view = view * invert;

		return view;
	}
	static Matrix4f getTranslationMatrix(Vector3f position)
	{
		Matrix4f mat;
		mat = Matrix4f::getIdentity();
		mat.values[12] = position.x;
		mat.values[13] = position.y;
		mat.values[14] = position.z;
		return mat;
	}
	static Matrix4f transformMatrix(Matrix4f mat, Vector3f position)
	{
		Matrix4f rotMat;
		rotMat = Matrix4f::getIdentity();
		rotMat.values[12] = position.x;
		rotMat.values[13] = position.y;
		rotMat.values[14] = position.z;
		Matrix4f result = rotMat * mat;
		return result;
	}
	static Matrix4f scaleMatrix(Matrix4f mat, Vector3f scale)
	{
		Matrix4f mats;
		mats = Matrix4f::getIdentity();
		mats.values[0] = scale.x;
		mats.values[5] = scale.y;
		mats.values[10] = scale.z;
		mat = mat * mats;
		return mat;
	}
	static Matrix4f rotateMatrix(Matrix4f mat, Vector3f rotation)
	{
		Quaternion qr = Quaternion::getIdentity();

		Quaternion xrot = Quaternion::getRotationQuaterion(Vector3f::getx(), rotation.x);
		Quaternion yrot = Quaternion::getRotationQuaterion(Vector3f::gety(), rotation.y);
		Quaternion zrot = Quaternion::getRotationQuaterion(Vector3f::getz(), rotation.z);

		qr = xrot * qr;
		qr = yrot * qr;
		qr = zrot * qr;

		Matrix4f m = Matrix4f::getIdentity();
		m.values[0] = 1.0f - 2.0f*qr.y*qr.y - 2.0f*qr.z*qr.z;
		m.values[1] = 2.0f*qr.x*qr.y - 2.0f*qr.z*qr.w;
		m.values[2] = 2.0f*qr.x*qr.z + 2.0f*qr.y*qr.w;
		m.values[3] = 0;

		m.values[4] = 2.0f*qr.x*qr.y + 2.0f*qr.z*qr.w;
		m.values[5] = 1.0f - 2.0f*qr.x*qr.x - 2.0f*qr.z*qr.z;
		m.values[6] = 2.0f*qr.y*qr.z - 2.0f*qr.x*qr.w;
		m.values[7] = 0;

		m.values[8] = 2.0f*qr.x*qr.z - 2.0f*qr.y*qr.w;
		m.values[9] = 2.0f*qr.y*qr.z + 2.0f*qr.x*qr.w;
		m.values[10] = 1.0f - 2.0f*qr.x*qr.x - 2.0f*qr.y*qr.y;
		m.values[11] = 0;

		//last column already set

		mat = mat * m;
		return mat;
	}
	Matrix4f operator*(const Matrix4f& m)
	{
		//THIS is matrix 1 and m is matrix 2
		Matrix4f mat;

		//Row 1
		mat.values[0] = (this->values[0] * m.values[0]) + (this->values[4] * m.values[1]) + (this->values[8] * m.values[2]) + (this->values[12] * m.values[3]);
		mat.values[4] = (this->values[0] * m.values[4]) + (this->values[4] * m.values[5]) + (this->values[8] * m.values[6]) + (this->values[12] * m.values[7]);
		mat.values[8] = (this->values[0] * m.values[8]) + (this->values[4] * m.values[9]) + (this->values[8] * m.values[10]) + (this->values[12] * m.values[11]);
		mat.values[12] = (this->values[0] * m.values[12]) + (this->values[4] * m.values[13]) + (this->values[8] * m.values[14]) + (this->values[12] * m.values[15]);

		//Row 2
		mat.values[1] = (this->values[1] * m.values[0]) + (this->values[5] * m.values[1]) + (this->values[9] * m.values[2]) + (this->values[13] * m.values[3]);
		mat.values[5] = (this->values[1] * m.values[4]) + (this->values[5] * m.values[5]) + (this->values[9] * m.values[6]) + (this->values[13] * m.values[7]);
		mat.values[9] = (this->values[1] * m.values[8]) + (this->values[5] * m.values[9]) + (this->values[9] * m.values[10]) + (this->values[13] * m.values[11]);
		mat.values[13] = (this->values[1] * m.values[12]) + (this->values[13] * m.values[9]) + (this->values[9] * m.values[14]) + (this->values[13] * m.values[15]);

		//Row 3
		mat.values[2] = (this->values[2] * m.values[0]) + (this->values[6] * m.values[1]) + (this->values[10] * m.values[2]) + (this->values[14] * m.values[3]);
		mat.values[6] = (this->values[2] * m.values[4]) + (this->values[6] * m.values[5]) + (this->values[10] * m.values[6]) + (this->values[14] * m.values[7]);
		mat.values[10] = (this->values[2] * m.values[8]) + (this->values[6] * m.values[9]) + (this->values[10] * m.values[10]) + (this->values[14] * m.values[11]);
		mat.values[14] = (this->values[2] * m.values[12]) + (this->values[6] * m.values[13]) + (this->values[10] * m.values[14]) + (this->values[14] * m.values[15]);

		//Row 4
		mat.values[3] = (this->values[3] * m.values[0]) + (this->values[7] * m.values[1]) + (this->values[11] * m.values[2]) + (this->values[15] * m.values[3]);
		mat.values[7] = (this->values[3] * m.values[4]) + (this->values[7] * m.values[5]) + (this->values[11] * m.values[6]) + (this->values[15] * m.values[7]);
		mat.values[11] = (this->values[3] * m.values[8]) + (this->values[7] * m.values[9]) + (this->values[11] * m.values[10]) + (this->values[15] * m.values[11]);
		mat.values[15] = (this->values[3] * m.values[12]) + (this->values[7] * m.values[13]) + (this->values[11] * m.values[14]) + (this->values[15] * m.values[15]);


		return mat;
	}
};