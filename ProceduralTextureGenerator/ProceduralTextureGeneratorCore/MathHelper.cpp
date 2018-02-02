#include "stdafx.h"
#include "MathHelper.h"


Vector2D GenerateGradient()
{
	float x;
	float y;
	float dot;
	do
	{
		x = rng.RandomUniform(-1.0f, 1.0f);
		y = rng.RandomUniform(-1.0f, 1.0f);
		dot = x * x + y * y;
	}
	while(dot > 1.0f || dot == 0.0f);

	return Vector2D(x, y);
}


float Dot(Vector2D u, Vector2D v)
{
	return u.x * v.x + u.y * v.y;
}


float Lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}


float Qerp(float x)
{
	return x * x * x * (x * (x * 6 - 15) + 10);
}