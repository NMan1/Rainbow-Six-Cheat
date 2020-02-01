#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <corecrt_math.h>

#define M_PI 3.14159265358979323846264338327950288419716939937510

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}

	float x;
	float y;
};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x;
	float y;
	float z;

	inline float Length()
	{
		float ls = x * x + y * y + z * z;
		return sqrt(ls);
	}


	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	void clamp()
	{
		if (this->x > 180.f)
			this->x -= 360.f;

		else if (this->x < -180.f)
			this->x += 360.f;

		if (this->z > 180.f)
			this->z -= 360.f;

		else if (this->z < -180.f)
			this->z += 360.f;

		if (this->x < -89.f)
			this->x = -89.f;

		if (this->x > 89.f)
			this->x = 89.f;

		while (this->z < -180.0f)
			this->z += 360.0f;

		while (this->z > 180.0f)
			this->z -= 360.0f;
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float number) const
	{
		return Vector3(x * number, y * number, z * number);
	}
};

class Vector4
{
public:
public:
	float mData[4];
	float w()
	{
		return mData[3];
	}
	float x()
	{
		return mData[0];
	}
	float y()
	{
		return mData[1];
	}
	float z()
	{
		return mData[2];
	}
};

#endif#pragma once
