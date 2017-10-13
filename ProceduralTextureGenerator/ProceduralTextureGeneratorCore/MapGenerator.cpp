#include "stdafx.h"
#include "MapGenerator.h"


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


MapGenerator::MapGenerator()
{

}


MapGenerator::~MapGenerator()
{

}


float MapGenerator::Dot(Vector2D u, Vector2D v)
{
	return u.x * v.x + u.y * v.y;
}


float MapGenerator::Lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}


float MapGenerator::Cerp(float x)
{
	return 0.25f + x * (1.5f + x * (-3 + 2 * x));
}


float MapGenerator::Qerp(float x)
{
	return x * x * x * (x * (x * 6 - 15) + 10);
}


void MapGenerator::Normalize()
{
	TextureMemory &generatedMap = *generatedMapPtr.get();

	float heightMin = generatedMap(0, 0, 0);
	float heightMax = heightMin;

	for(int i = 0; i < generatedMap.GetSize(); i++)
	{
		for(int j = 0; j < generatedMap.GetSize(); j++)
		{
			float height = generatedMap(i, j, 0);
			if(height < heightMin)
			{
				heightMin = height;
			}
			else if(height > heightMax)
			{
				heightMax = height;
			}
		}
		
	}

	float dh = heightMax - heightMin;
	for(int i = 0; i < generatedMap.GetSize(); i++)
	{
		for(int j = 0; j < generatedMap.GetSize(); j++)
		{
			generatedMap(i, j, 0) = (generatedMap(i, j, 0) - heightMin) / dh;
		}
	}
}


Vector2D MapGenerator::GenerateGradient()
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


shared_ptr<TextureMemory> MapGenerator::PerlinNoise(int size_, int octaves_, int gridSize_, float persistence_)
{
	size = size_;

	generatedMapPtr = make_shared<TextureMemory>(GRAYSCALE, size_);
	TextureMemory &generatedMap = *generatedMapPtr.get();

	for(int k = 0; k < octaves_; k++)
	{
		int gridSize = gridSize_ * (int) pow(2, k) + 1;
		int nodesNum = gridSize * gridSize;
		float amplitude = pow(persistence_, k);

		std::vector<Vector2D> gradients;
		gradients = std::vector<Vector2D>(nodesNum);
		for(int i = 0; i < nodesNum; i++)
		{
			gradients[i] = GenerateGradient();
		}

		for(int i = 0; i < size; i++)
		{
			Vector2D dir;

			float z = (float) i / (size - 1) * (gridSize - 1);
			int zNode = (int) z < (gridSize - 1) ? (int) z : (gridSize - 2);
			dir.y = z - zNode;
			float zQerp = Qerp(dir.y);

			int zPad0 = zNode * gridSize;
			int zPad1 = (zNode + 1) * gridSize;
			for(int j = 0; j < size; j++)
			{
				float x = (float) j / (size - 1) * (gridSize - 1);
				int xNode = (int) x < (gridSize - 1) ? (int) x : (gridSize - 2);
				dir.x = x - xNode;
				float xQerp = Qerp(dir.x);

				Vector2D grad11 = gradients[zPad0 + xNode];
				Vector2D grad12 = gradients[zPad0 + xNode + 1];
				Vector2D grad21 = gradients[zPad1 + xNode];
				Vector2D grad22 = gradients[zPad1 + xNode + 1];

				Vector2D dir11 = dir;
				Vector2D dir12 = dir - Vector2D(1.0f, 0.0f);
				Vector2D dir21 = dir - Vector2D(0.0f, 1.0f);
				Vector2D dir22 = dir - Vector2D(1.0f, 1.0f);

				float dot11 = Dot(grad11, dir11);
				float dot12 = Dot(grad12, dir12);
				float dot21 = Dot(grad21, dir21);
				float dot22 = Dot(grad22, dir22);

				float height1 = Lerp(dot11, dot12, xQerp);
				float height2 = Lerp(dot21, dot22, xQerp);
				float height = Lerp(height1, height2, zQerp);

				generatedMap(i, j, 0) += height * amplitude;
			}
		}
	}

	Normalize();

	return generatedMapPtr;
}


shared_ptr<TextureMemory> MapGenerator::GenerateAlbedoMap(int size_, XMFLOAT4 value)
{
	shared_ptr<TextureMemory> albedoMapPtr = make_shared<TextureMemory>(COLOR, size_);
	TextureMemory &albedoMap = *albedoMapPtr.get();

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			albedoMap(i, j, 0) = value.x;
			albedoMap(i, j, 1) = value.y;
			albedoMap(i, j, 2) = value.z;
			albedoMap(i, j, 3) = value.w;
		}
	}

	return albedoMapPtr;
}

shared_ptr<TextureMemory> MapGenerator::GenerateMetallicMap(int size_, float value)
{
	shared_ptr<TextureMemory> metallicMapPtr = make_shared<TextureMemory>(GRAYSCALE, size_);
	TextureMemory &metallicMap = *metallicMapPtr.get();

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			metallicMap(i, j, 0) = value;
		}
	}

	return metallicMapPtr;
}

shared_ptr<TextureMemory> MapGenerator::GenerateRoughnessMap(int size_, float value)
{
	shared_ptr<TextureMemory> roughnessMapPtr = make_shared<TextureMemory>(GRAYSCALE, size_);
	TextureMemory &roughnessMap = *roughnessMapPtr.get();

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			roughnessMap(i, j, 0) = value;
		}
	}

	return roughnessMapPtr;
}


shared_ptr<TextureMemory> MapGenerator::GenerateNormalMap(int size_, XMFLOAT4 value)
{
	shared_ptr<TextureMemory> normalMapPtr = make_shared<TextureMemory>(COLOR, size_);
	TextureMemory &normalMap = *normalMapPtr.get();

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			normalMap(i, j, 0) = value.x;
			normalMap(i, j, 1) = value.y;
			normalMap(i, j, 2) = value.z;
			normalMap(i, j, 3) = value.w;
		}
	}

	return normalMapPtr;
}


void MapGenerator::GenerateRustyIronMaps(int size_, shared_ptr<TextureMemory> &albedoMapPtr, shared_ptr<TextureMemory> &metallicMapPtr, shared_ptr<TextureMemory> &roughnessMapPtr)
{
	albedoMapPtr = make_shared<TextureMemory>(COLOR, size_);
	metallicMapPtr = make_shared<TextureMemory>(GRAYSCALE, size_);
	roughnessMapPtr = make_shared<TextureMemory>(GRAYSCALE, size_);

	TextureMemory &albedoMap = *albedoMapPtr.get();
	TextureMemory &metallicMap = *metallicMapPtr.get();
	TextureMemory &roughnessMap = *roughnessMapPtr.get();

	shared_ptr<TextureMemory> rustNoisePtr = PerlinNoise(size_, 8, 2, 0.8f);
	TextureMemory &rustNoise = *rustNoisePtr.get();

	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			rustNoise(i, j, 0) = min(max(-1.75 + 2.75 * rustNoise(i, j, 0), 0.0f), 1.0f);
		}
	}

	Gradient gradient[72] =
	{
		Gradient(145, 97, 77, 0.00f),
		Gradient(153, 110, 94, 0.01f),
		Gradient(133, 94, 79, 0.02f),
		Gradient(142, 107, 91, 0.04f),
		Gradient(114, 79, 60, 0.05f),
		Gradient(126, 77, 63, 0.09f),
		Gradient(116, 61, 41, 0.12f),
		Gradient(141, 86, 65, 0.13f),
		Gradient(138, 87, 58, 0.15f),
		Gradient(193, 139, 111, 0.16f),
		Gradient(169, 118, 89, 0.17f),
		Gradient(120, 69, 42, 0.20f),
		Gradient(161, 111, 84, 0.21f),
		Gradient(149, 97, 73, 0.22f),
		Gradient(181, 131, 106, 0.25f),
		Gradient(152, 102, 77, 0.26f),
		Gradient(169, 119, 94, 0.27f),
		Gradient(121, 67, 41, 0.28f),
		Gradient(89, 46, 27, 0.30f),
		Gradient(107, 70, 51, 0.31f),
		Gradient(100, 67, 50, 0.32f),
		Gradient(122, 93, 77, 0.33f),
		Gradient(90, 58, 43, 0.34f),
		Gradient(120, 69, 50, 0.35f),
		Gradient(102, 44, 24, 0.36f),
		Gradient(131, 65, 43, 0.37f),
		Gradient(87, 20, 0, 0.38f),
		Gradient(94, 30, 3, 0.39f),
		Gradient(149, 83, 57, 0.41f),
		Gradient(104, 28, 9, 0.42f),
		Gradient(123, 46, 26, 0.43f),
		Gradient(97, 22, 1, 0.44f),
		Gradient(91, 35, 11, 0.46f),
		Gradient(119, 64, 40, 0.48f),
		Gradient(91, 30, 9, 0.49f),
		Gradient(133, 72, 48, 0.50f),
		Gradient(109, 52, 22, 0.51f),
		Gradient(128, 75, 50, 0.53f),
		Gradient(155, 100, 75, 0.54f),
		Gradient(147, 90, 63, 0.55f),
		Gradient(109, 46, 15, 0.58f),
		Gradient(133, 81, 60, 0.62f),
		Gradient(116, 61, 40, 0.65f),
		Gradient(143, 89, 65, 0.66f),
		Gradient(110, 50, 26, 0.69f),
		Gradient(142, 81, 56, 0.70f),
		Gradient(146, 83, 57, 0.71f),
		Gradient(114, 47, 20, 0.72f),
		Gradient(155, 85, 59, 0.73f),
		Gradient(128, 58, 32, 0.74f),
		Gradient(162, 93, 64, 0.75f),
		Gradient(137, 68, 39, 0.76f),
		Gradient(157, 88, 59, 0.77f),
		Gradient(186, 115, 87, 0.78f),
		Gradient(181, 110, 82, 0.79f),
		Gradient(111, 37, 8, 0.80f),
		Gradient(100, 24, 0, 0.81f),
		Gradient(145, 69, 43, 0.82f),
		Gradient(120, 43, 17, 0.84f),
		Gradient(152, 75, 49, 0.85f),
		Gradient(122, 44, 21, 0.86f),
		Gradient(151, 75, 52, 0.88f),
		Gradient(133, 53, 30, 0.89f),
		Gradient(170, 88, 64, 0.90f),
		Gradient(133, 51, 27, 0.91f),
		Gradient(185, 103, 79, 0.92f),
		Gradient(166, 84, 60, 0.93f),
		Gradient(119, 39, 14, 0.94f),
		Gradient(107, 28, 9, 0.95f),
		Gradient(87, 9, 0, 0.96f),
		Gradient(135, 59, 35, 0.98f),
		Gradient(110, 35, 12, 1.00f)
	};

	shared_ptr<TextureMemory> gradientMapPtr = make_shared<TextureMemory>(COLOR, size_);
	TextureMemory &gradientMap = *gradientMapPtr.get();
	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			int gradientIndex = 0;
			while(gradient[gradientIndex].value < rustNoise(i, j, 0))
			{
				gradientIndex++;
			}

			if(gradientIndex == 0)
			{
				gradientMap(i, j, 0) = gradient[0].color.x;
				gradientMap(i, j, 1) = gradient[0].color.y;
				gradientMap(i, j, 2) = gradient[0].color.z;
				gradientMap(i, j, 3) = gradient[0].color.w;
			}
			else if(gradientIndex > 71)
			{
				gradientMap(i, j, 0) = gradient[71].color.x;
				gradientMap(i, j, 1) = gradient[71].color.y;
				gradientMap(i, j, 2) = gradient[71].color.z;
				gradientMap(i, j, 3) = gradient[71].color.w;
			}
			else
			{
				float k = (gradient[gradientIndex].value - rustNoise(i, j, 0)) / (gradient[gradientIndex].value - gradient[gradientIndex - 1].value);

				gradientMap(i, j, 0) = (1 - k) * gradient[gradientIndex - 1].color.x + k * gradient[gradientIndex].color.x;
				gradientMap(i, j, 1) = (1 - k) * gradient[gradientIndex - 1].color.y + k * gradient[gradientIndex].color.y;
				gradientMap(i, j, 2) = (1 - k) * gradient[gradientIndex - 1].color.z + k * gradient[gradientIndex].color.z;
				gradientMap(i, j, 3) = (1 - k) * gradient[gradientIndex - 1].color.w + k * gradient[gradientIndex].color.w;
			}
		}
	}

	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			float k = pow(rustNoise(i, j, 0), 0.5);

			albedoMap(i, j, 0) = (1 - k) * 0.7686f + k * gradientMap(i, j, 0);
			albedoMap(i, j, 1) = (1 - k) * 0.7803f + k * gradientMap(i, j, 1);
			albedoMap(i, j, 2) = (1 - k) * 0.7803f + k * gradientMap(i, j, 2);
			albedoMap(i, j, 3) = 1.0f;
		}
	}

	shared_ptr<TextureMemory> grazeNoisePtr = PerlinNoise(size_, 8, 1, 1.5f);
	TextureMemory &grazeNoise = *grazeNoisePtr.get();

	shared_ptr<TextureMemory> metalRoughnessPtr = make_shared<TextureMemory>(GRAYSCALE, size_);
	TextureMemory &metalRoughness = *metalRoughnessPtr.get();;
	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			metalRoughness(i, j, 0) = 0.35f + 0.2f * min(max(-3 + 4 * grazeNoise(i, j, 0), 0.0f), 1.0f);
		}
	}

	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			roughnessMap(i, j, 0) = max(metalRoughness(i, j, 0), rustNoise(i, j, 0));
			metallicMap(i, j, 0) = max(1.0f - rustNoise(i, j, 0), 0.0f);
		}
	}
}