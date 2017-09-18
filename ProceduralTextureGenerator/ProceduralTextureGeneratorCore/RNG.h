#pragma once


#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "DirectXDependencies.h"

using namespace DirectX;


class RNG
{
	public:

	RNG();
	~RNG();

	float RandomUniform();
	float RandomUniform(float a, float b);
	float RandomNormal();
	float RandomNormal(float mean, float variance);

	XMFLOAT2 RandomNormal2();
};