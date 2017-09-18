#include "stdafx.h"
#include "RNG.h"



RNG::RNG()
{
	srand(time(NULL));
}


RNG::~RNG()
{

}


float RNG::RandomUniform()
{
	return (float) rand() / RAND_MAX;
}


float RNG::RandomUniform(float a, float b)
{
	return a + RandomUniform() * (b - a);
}


float RNG::RandomNormal()
{
	float xr;
	float yr;
	float s;

	do
	{
		xr = RandomUniform(-1.0f, 1.0f);
		yr = RandomUniform(-1.0f, 1.0f);
		s = xr * xr + yr * yr;
	}
	while(s >= 1.0f);

	s = sqrt(-2.0f * log(s) / s);

	return xr * s;
}


float RNG::RandomNormal(float mean, float variance)
{
	return mean + RandomNormal() * variance;
}


XMFLOAT2 RNG::RandomNormal2()
{
	float xr;
	float yr;
	float s;

	do
	{
		xr = RandomUniform(-1.0f, 1.0f);
		yr = RandomUniform(-1.0f, 1.0f);
		s = xr * xr + yr * yr;
	}
	while(s >= 1.0f);

	s = sqrt(-2.0f * log(s) / s);

	return XMFLOAT2(xr * s, yr * s);
}