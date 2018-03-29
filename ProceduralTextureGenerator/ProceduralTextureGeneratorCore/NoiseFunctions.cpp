#include "stdafx.h"
#include "NoiseFunctions.h"


TextureMemoryPtr PerlinNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int octaves, int gridStartingSize, float persistence)
{
	TextureMemoryPtr perlinNoiseTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	vector<float> noise(resolution * resolution, 0.0f);

	vector<vector<Vector2D>> gradients(octaves);
	vector<int> gridSize(octaves);
	vector<float> amplitude(octaves);

	for(int k = 0; k < octaves; k++)
	{
		gridSize[k] = gridStartingSize * (int) exp(k) + 1;
		amplitude[k] = pow(persistence, k);

		gradients[k].resize(gridSize[k] * gridSize[k]);
		for(int i = 0; i < gradients[k].size(); i++)
		{
			gradients[k][i] = GenerateGradient();
		}
	}

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			for(int k = 0; k < octaves; k++)
			{
				Vector2D dir;

				float x = (float)j / (resolution - 1) * (gridSize[k] - 1);
				int xNode = (int)x < (gridSize[k] - 1) ? (int)x : (gridSize[k] - 2);
				dir.x = x - xNode;
				float xQerp = Qerp(dir.x);

				float z = (float) i / (resolution - 1) * (gridSize[k] - 1);
				int zNode = (int) z < (gridSize[k] - 1) ? (int) z : (gridSize[k] - 2);
				dir.y = z - zNode;
				float zQerp = Qerp(dir.y);

				int zPad0 = zNode * gridSize[k];
				int zPad1 = (zNode + 1) * gridSize[k];

				Vector2D grad11 = gradients[k][zPad0 + xNode];
				Vector2D grad12 = gradients[k][zPad0 + xNode + 1];
				Vector2D grad21 = gradients[k][zPad1 + xNode];
				Vector2D grad22 = gradients[k][zPad1 + xNode + 1];

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

				noise[i * resolution + j] += height * amplitude[k];
			}
		}
	}

	float heightMin = noise[0];
	float heightMax = heightMin;

	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float height = noise[i * resolution + j];
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

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			perlinNoiseTexturePtr->SetValue(i, j, XMFLOAT2((noise[i * resolution + j] - heightMin) / dh, 1.0));
		}
	}

	return perlinNoiseTexturePtr;
}


TextureMemoryPtr WorleyNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int octaves, int sitesStartingNum, float persistence, UINT distanceType, float exponent)
{
	TextureMemoryPtr worleyNoiseTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	vector<float> noise(resolution * resolution, 0.0f);

	vector<vector<XMFLOAT2>> sites(octaves);
	vector<float> amplitude(octaves);

	random_device rd;
	mt19937 rng(rd());
	uniform_real_distribution<> uniformRNG(0.0, 1.0);
	
	for(int k = 0; k < octaves; k++)
	{
		sites[k].resize(sitesStartingNum * (int) exp2(k));
		for(int i = 0; i < sites[k].size(); i++)
		{
			sites[k][i] = XMFLOAT2(uniformRNG(rng), uniformRNG(rng));
		}

		amplitude[k] = pow(persistence, k);
	}

	switch(distanceType)
	{
		//1 point
		case 1:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMFLOAT2 uv = XMFLOAT2((float) i / resolution, (float) j / resolution);

					for(int k = 0; k < octaves; k++)
					{
						double distance = 100;
						for(int l = 0; l < sites[k].size(); l++)
						{
							float x = uv.x - sites[k][l].x > 0.5f ? sites[k][l].x + 1.0f : uv.x - sites[k][l].x < -0.5f ? sites[k][l].x - 1.0f : sites[k][l].x;
							float y = uv.y - sites[k][l].y > 0.5f ? sites[k][l].y + 1.0f : uv.y - sites[k][l].y < -0.5f ? sites[k][l].y - 1.0f : sites[k][l].y;
							
							double distance_ = pow(pow(abs(x - uv.x), exponent) + pow(abs(y - uv.y), exponent), 1.0 / exponent);
							if(distance_ < distance)
							{
								distance = distance_;
							}
						}

						noise[i * resolution + j] += distance * amplitude[k];
					}
				}
			}
			break;
		}
		//2 points
		case 2:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMFLOAT2 uv = XMFLOAT2((float) i / resolution, (float) j / resolution);

					for(int k = 0; k < octaves; k++)
					{
						double distance1 = 100;
						double distance2 = 101;
						for(int l = 0; l < sites[k].size(); l++)
						{
							float x = uv.x - sites[k][l].x > 0.5f ? sites[k][l].x + 1.0f : uv.x - sites[k][l].x < -0.5f ? sites[k][l].x - 1.0f : sites[k][l].x;
							float y = uv.y - sites[k][l].y > 0.5f ? sites[k][l].y + 1.0f : uv.y - sites[k][l].y < -0.5f ? sites[k][l].y - 1.0f : sites[k][l].y;

							double distance_ = pow(pow(abs(x - uv.x), exponent) + pow(abs(y - uv.y), exponent), 1.0 / exponent);
							if(distance_ < distance1)
							{
								distance2 = distance1;
								distance1 = distance_;
							}
							else if(distance_ < distance2)
							{
								distance2 = distance_;
							}
						}

						noise[i * resolution + j] += (distance2 - distance1) * amplitude[k];
					}
				}
			}
			break;
		}
	}

	float heightMin = noise[0];
	float heightMax = heightMin;

	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float height = noise[i * resolution + j];
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

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			worleyNoiseTexturePtr->SetValue(i, j, XMFLOAT2((noise[i * resolution + j] - heightMin) / dh, 1.0));
		}
	}

	return worleyNoiseTexturePtr;
}
