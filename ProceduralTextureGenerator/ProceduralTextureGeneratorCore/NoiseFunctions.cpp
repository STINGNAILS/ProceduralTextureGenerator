#include "stdafx.h"
#include "NoiseFunctions.h"


TextureMemoryPtr PerlinNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int seed, float persistence, int minimalOctave, int maximalOctave)
{
	TextureMemoryPtr perlinNoiseTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	vector<float> noise(resolution * resolution, 0.0f);

	int octavesNum = maximalOctave - minimalOctave + 1;
	vector<vector<XMFLOAT2>> gradients(octavesNum);
	vector<int> gridSize(octavesNum);
	vector<float> amplitude(octavesNum);

	mt19937 rng(seed);
	uniform_real_distribution<> uniformRNG(-1.0, 1.0);

	for(int k = 0; k < octavesNum; k++)
	{
		gridSize[k] = (int) exp2(minimalOctave + k - 1) + 1;
		amplitude[k] = pow(persistence, k);

		gradients[k].resize(gridSize[k] * gridSize[k]);
		for(int i = 0; i < gradients[k].size(); i++)
		{
			gradients[k][i] = XMFLOAT2(uniformRNG(rng), uniformRNG(rng));
		}
	}

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			for(int k = 0; k < octavesNum; k++)
			{
				XMFLOAT2 dir;

				float x = (float) j / (resolution - 1) * (gridSize[k] - 1);
				int xNode = (int) x < (gridSize[k] - 1) ? (int) x : (gridSize[k] - 2);
				dir.x = x - xNode;
				float xQerp = Qerp(dir.x);

				float z = (float) i / (resolution - 1) * (gridSize[k] - 1);
				int zNode = (int) z < (gridSize[k] - 1) ? (int) z : (gridSize[k] - 2);
				dir.y = z - zNode;
				float zQerp = Qerp(dir.y);

				int zPad0 = zNode * gridSize[k];
				int zPad1 = (zNode + 1) * gridSize[k];

				XMFLOAT2 grad11 = gradients[k][zPad0 + xNode];
				XMFLOAT2 grad12 = gradients[k][zPad0 + xNode + 1];
				XMFLOAT2 grad21 = gradients[k][zPad1 + xNode];
				XMFLOAT2 grad22 = gradients[k][zPad1 + xNode + 1];

				XMFLOAT2 dir11 = dir;
				XMFLOAT2 dir12 = XMFLOAT2(dir.x - 1.0f, dir.y);
				XMFLOAT2 dir21 = XMFLOAT2(dir.x, dir.y - 1.0f);
				XMFLOAT2 dir22 = XMFLOAT2(dir.x - 1.0f, dir.y - 1.0f);

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

	float valueMin = noise[0];
	float valueMax = valueMin;

	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float value = noise[i * resolution + j];
			if(value < valueMin)
			{
				valueMin = value;
			}
			else if(value > valueMax)
			{
				valueMax = value;
			}
		}

	}

	float dv = valueMax - valueMin;

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			perlinNoiseTexturePtr->SetValue(i, j, XMFLOAT2((noise[i * resolution + j] - valueMin) / dv, 1.0));
		}
	}

	return perlinNoiseTexturePtr;
}


TextureMemoryPtr WorleyNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int seed, int sitesNum, int patternType, int distanceType, float borderWidth)
{
	TextureMemoryPtr worleyNoiseTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	vector<float> noise(resolution * resolution, 0.0f);

	int gridSize = (int) floorf(sqrt(sitesNum / 5.0f));
	vector<vector<XMFLOAT2>> sites(gridSize * gridSize);

	int neighbourOffset0 = gridSize < 3 ? 0 : -1;
	int neighbourOffset1 = gridSize < 2 ? 0 : 1;

	mt19937 rng(seed);
	uniform_real_distribution<> uniformRNG(0.0, 1.0);
	
	for(int k = 0; k < sitesNum; k++)
	{
		XMFLOAT2 site(uniformRNG(rng), uniformRNG(rng));

		int xCell = (int) min(site.x * gridSize, gridSize - 1);
		int yCell = (int) min(site.y * gridSize, gridSize - 1);

		sites[yCell * gridSize + xCell].push_back(site);
	}

	switch(patternType)
	{
		//F1
		case 1:
		{
			switch(distanceType)
			{
				//Manhattan
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = abs(x - xSite) + abs(y - ySite);
										if(distance_ < distance)
										{
											distance = distance_;
										}
									}
								}
							}

							noise[i * resolution + j] = distance;
						}
					}

					break;
				}
				//Euclidean
				case 2:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = sqrt((x - xSite) * (x - xSite) + (y - ySite) * (y - ySite));
										if(distance_ < distance)
										{
											distance = distance_;
										}
									}
								}
							}

							noise[i * resolution + j] = distance;
						}
					}

					break;
				}
				//Chebyshev
				case 3:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = max(abs(x - xSite), abs(y - ySite));
										if(distance_ < distance)
										{
											distance = distance_;
										}
									}
								}
							}

							noise[i * resolution + j] = distance;
						}
					}

					break;
				}
			}
			
			break;
		}
		//F2
		case 2:
		{
			switch(distanceType)
			{
				//Manhattan
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = abs(x - xSite) + abs(y - ySite);
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
								}
							}

							noise[i * resolution + j] = distance2;
						}
					}

					break;
				}
				//Euclidean
				case 2:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = sqrt((x - xSite) * (x - xSite) + (y - ySite) * (y - ySite));
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
								}
							}

							noise[i * resolution + j] = distance2;
						}
					}

					break;
				}
				//Chebyshev
				case 3:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = max(abs(x - xSite), abs(y - ySite));
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
								}
							}

							noise[i * resolution + j] = distance2;
						}
					}

					break;
				}
			}

			break;
		}
		//F2 - F1
		case 3:
		{
			switch(distanceType)
			{
				//Manhattan
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = abs(x - xSite) + abs(y - ySite);
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
								}
							}

							noise[i * resolution + j] = distance2 - distance1;
						}
					}

					break;
				}
				//Euclidean
				case 2:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = sqrt((x - xSite) * (x - xSite) + (y - ySite) * (y - ySite));
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
								}
							}

							noise[i * resolution + j] = distance2 - distance1;
						}
					}

					break;
				}
				//Chebyshev
				case 3:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = max(abs(x - xSite), abs(y - ySite));
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
								}
							}

							noise[i * resolution + j] = distance2 - distance1;
						}
					}

					break;
				}
			}

			break;
		}
		//Border
		case 4:
		{
			switch(distanceType)
			{
				//Manhattan
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = abs(x - xSite) + abs(y - ySite);
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
								}
							}

							noise[i * resolution + j] = (distance2 - distance1) < borderWidth ? 1.0f : 0.0f;
						}
					}

					break;
				}
				//Euclidean
				case 2:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							double distance3 = 102;
							double distance4 = 103;
							XMFLOAT2 p1;
							XMFLOAT2 p2;
							XMFLOAT2 p3;
							XMFLOAT2 p4;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = sqrt((x - xSite) * (x - xSite) + (y - ySite) * (y - ySite));
										if(distance_ < distance1)
										{
											distance4 = distance3;
											distance3 = distance2;
											distance2 = distance1;
											distance1 = distance_;
											p4 = p3;
											p3 = p2;
											p2 = p1;
											p1 = XMFLOAT2(xSite, ySite);
										}
										else if(distance_ < distance2)
										{
											distance4 = distance3;
											distance3 = distance2;
											distance2 = distance_;
											p4 = p3;
											p3 = p2;
											p2 = XMFLOAT2(xSite, ySite);
										}
										else if(distance_ < distance3)
										{
											distance4 = distance3;
											distance3 = distance_;
											p4 = p3;
											p3 = XMFLOAT2(xSite, ySite);
										}
										else if(distance_ < distance4)
										{
											distance4 = distance_;
											p4 = XMFLOAT2(xSite, ySite);
										}
									}
								}
							}

							double a2 = p2.x - p1.x;
							double b2 = p2.y - p1.y;
							double x02 = x - (p1.x + p2.x) / 2.0f;
							double y02 = y - (p1.y + p2.y) / 2.0f;
							double r2 = abs(a2 * x02 + b2 * y02) / sqrt(a2 * a2 + b2 * b2);

							double a3 = p3.x - p1.x;
							double b3 = p3.y - p1.y;
							double x03 = x - (p1.x + p3.x) / 2.0f;
							double y03 = y - (p1.y + p3.y) / 2.0f;
							double r3 = abs(a3 * x03 + b3 * y03) / sqrt(a3 * a3 + b3 * b3);

							double a4 = p4.x - p1.x;
							double b4 = p4.y - p1.y;
							double x04 = x - (p1.x + p4.x) / 2.0f;
							double y04 = y - (p1.y + p4.y) / 2.0f;
							double r4 = abs(a4 * x04 + b4 * y04) / sqrt(a4 * a4 + b4 * b4);

							noise[i * resolution + j] = (r2 < borderWidth / 2.0f || r3 < borderWidth / 2.0f || r4 < borderWidth / 2.0f) ? 1.0f : 0.0f;
						}
					}

					break;
				}
				//Chebyshev
				case 3:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance1 = 100;
							double distance2 = 101;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = max(abs(x - xSite), abs(y - ySite));
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
								}
							}

							noise[i * resolution + j] = (distance2 - distance1) < borderWidth ? 1.0f : 0.0f;
						}
					}

					break;
				}
			}

			break;
		}
		//Random color
		case 5:
		{
			vector<vector<float>> siteColors(sites.size());

			mt19937 rngColor(seed + 1);
			uniform_real_distribution<> uniformRNGColor(0.0, 1.0);

			for(int i = 0; i < sites.size(); i++)
			{
				siteColors[i].resize(sites[i].size());

				for(int j = 0; j < siteColors[i].size(); j++)
				{
					siteColors[i][j] = uniformRNGColor(rngColor);
				}
			}

			switch(distanceType)
			{
				//Manhattan
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							float color = 0.0f;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = abs(x - xSite) + abs(y - ySite);
										if(distance_ < distance)
										{
											distance = distance_;
											color = siteColors[sitesCellIndex][m];
										}
									}
								}
							}

							noise[i * resolution + j] = color;
						}
					}

					break;
				}
				//Euclidean
				case 2:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							float color = 0.0f;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = sqrt((x - xSite) * (x - xSite) + (y - ySite) * (y - ySite));
										if(distance_ < distance)
										{
											distance = distance_;
											color = siteColors[sitesCellIndex][m];
										}
									}
								}
							}

							noise[i * resolution + j] = color;
						}
					}

					break;
				}
				//Chebyshev
				case 3:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						float y = (float) i / resolution;
						int yCell = (int) min(y * gridSize, gridSize - 1);

						for(int j = 0; j < resolution; j++)
						{
							float x = (float) j / resolution;
							int xCell = (int) min(x * gridSize, gridSize - 1);

							double distance = 100;
							float color = 0.0f;
							for(int k = neighbourOffset0; k <= neighbourOffset1; k++)
							{
								int ykCell = yCell + k;
								ykCell = ykCell >= 0 ? (ykCell < gridSize ? ykCell : (ykCell - gridSize)) : (ykCell + gridSize);

								for(int l = neighbourOffset0; l <= neighbourOffset1; l++)
								{
									int xlCell = xCell + l;
									xlCell = xlCell >= 0 ? (xlCell < gridSize ? xlCell : (xlCell - gridSize)) : (xlCell + gridSize);

									int sitesCellIndex = ykCell * gridSize + xlCell;
									for(int m = 0; m < sites[sitesCellIndex].size(); m++)
									{
										float xSite = (x - sites[sitesCellIndex][m].x) > 0.5f ? (sites[sitesCellIndex][m].x + 1.0f) : (x - sites[sitesCellIndex][m].x) < -0.5f ? (sites[sitesCellIndex][m].x - 1.0f) : sites[sitesCellIndex][m].x;
										float ySite = (y - sites[sitesCellIndex][m].y) > 0.5f ? (sites[sitesCellIndex][m].y + 1.0f) : (y - sites[sitesCellIndex][m].y) < -0.5f ? (sites[sitesCellIndex][m].y - 1.0f) : sites[sitesCellIndex][m].y;

										double distance_ = max(abs(x - xSite), abs(y - ySite));
										if(distance_ < distance)
										{
											distance = distance_;
											color = siteColors[sitesCellIndex][m];
										}
									}
								}
							}

							noise[i * resolution + j] = color;
						}
					}

					break;
				}
			}

			break;
		}
	}

	float valueMin = noise[0];
	float valueMax = valueMin;

	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float value = noise[i * resolution + j];
			if(value < valueMin)
			{
				valueMin = value;
			}
			else if(value > valueMax)
			{
				valueMax = value;
			}
		}

	}

	float dv = valueMax - valueMin;

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			worleyNoiseTexturePtr->SetValue(i, j, XMFLOAT2((noise[i * resolution + j] - valueMin) / dv, 1.0));
		}
	}

	return worleyNoiseTexturePtr;
}
