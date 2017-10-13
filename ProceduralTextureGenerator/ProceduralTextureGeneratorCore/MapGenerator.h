#pragma once


#include <memory>
#include "DirectXDependencies.h"
#include "TextureMemory.h"
#include "RNG.h"


using namespace std;
using namespace DirectX;


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


struct Gradient
{
	XMFLOAT4 color;
	float value;


	Gradient()
	{

	}


	Gradient(int r, int g, int b, float value_)
	{
		color.x = (float) r / 255.0f;
		color.y = (float) g / 255.0f;
		color.z = (float) b / 255.0f;
		color.w = 1.0f;
		value = value_;
	}
};


class MapGenerator
{
	private:

	int size;
	shared_ptr<TextureMemory> generatedMapPtr;

	RNG rng;

	float Dot(Vector2D u, Vector2D v);
	float Lerp(float a, float b, float t);
	float Cerp(float x);
	float Qerp(float x);
	void Normalize();

	Vector2D GenerateGradient();

	public:

	MapGenerator();
	~MapGenerator();

	shared_ptr<TextureMemory> PerlinNoise(int _size, int _octaves, int _gridSize, float _persistence);

	shared_ptr<TextureMemory> GenerateAlbedoMap(int size_, XMFLOAT4 value);
	shared_ptr<TextureMemory> GenerateMetallicMap(int size_, float value);
	shared_ptr<TextureMemory> GenerateRoughnessMap(int size_, float value);
	shared_ptr<TextureMemory> GenerateNormalMap(int size_, XMFLOAT4 value);

	void GenerateRustyIronMaps(int size_, shared_ptr<TextureMemory> &albedoMapPtr, shared_ptr<TextureMemory> &metallicMapPtr, shared_ptr<TextureMemory> &roughnessMapPtr);
};