#pragma once

#include "RNG.h"


static RNG rng;


struct Vector2D
{
	float x;
	float y;


	Vector2D()
	{
		x = 0;
		y = 0;
	}

	Vector2D(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	void operator+=(Vector2D v)
	{
		x += v.x;
		y += v.y;
	}
};


static Vector2D operator+(Vector2D u, Vector2D v)
{
	return Vector2D(u.x + v.x, u.y + v.y);
}


static Vector2D operator-(Vector2D u, Vector2D v)
{
	return Vector2D(u.x - v.x, u.y - v.y);
}


static Vector2D operator*(float f, Vector2D v)
{
	return Vector2D(v.x * f, v.y * f);
}


static Vector2D operator*(Vector2D v, float f)
{
	return Vector2D(v.x * f, v.y * f);
}

Vector2D GenerateGradient();


struct Grad
{
	XMFLOAT4 color;
	float value;


	Grad()
	{

	}


	Grad(int r, int g, int b, float value_)
	{
		color.x = (float) r / 255.0f;
		color.y = (float) g / 255.0f;
		color.z = (float) b / 255.0f;
		color.w = 1.0f;
		value = value_;
	}
};


float Dot(Vector2D u, Vector2D v);
float Lerp(float a, float b, float t);
float Qerp(float x);