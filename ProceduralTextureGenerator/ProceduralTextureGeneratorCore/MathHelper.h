#pragma once

#include "DirectXDependencies.h"


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


float Dot(XMFLOAT2 u, XMFLOAT2 v);
float Lerp(float a, float b, float t);
float Qerp(float x);