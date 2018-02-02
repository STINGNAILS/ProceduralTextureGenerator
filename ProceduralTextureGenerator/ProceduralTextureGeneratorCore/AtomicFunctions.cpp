#include "stdafx.h"
#include "AtomicFunctions.h"


TextureMemoryPtr BaseColor(TextureMemoryPtr inputTexturePtr, int size)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(size, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	return inputTexturePtr;
}


TextureMemoryPtr BaseGrayscale(TextureMemoryPtr inputTexturePtr, int size)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(size, GRAYSCALE, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	return inputTexturePtr;
}


TextureMemoryPtr UniformColor(int size, TextureType textureType, XMFLOAT4 color)
{
	TextureMemoryPtr uniformColorTexturePtr = make_shared<TextureMemory>(textureType, size);
	TextureMemory &uniformColorTexture = *uniformColorTexturePtr.get();

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					uniformColorTexture(i, j, 0) = color.x;
					uniformColorTexture(i, j, 1) = color.w;
				}
			}
			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					uniformColorTexture(i, j, 0) = color.x;
					uniformColorTexture(i, j, 1) = color.y;
					uniformColorTexture(i, j, 2) = color.z;
					uniformColorTexture(i, j, 3) = color.w;
				}
			}
			break;
		}
	}

	return uniformColorTexturePtr;
}


TextureMemoryPtr Blend(TextureMemoryPtr foregroundTexturePtr, TextureMemoryPtr backgroundTexturePtr, TextureMemoryPtr blendCoefficientTexturePtr, int size, int blendMode, float k)
{
	if(foregroundTexturePtr.get() == nullptr || backgroundTexturePtr.get() == nullptr)
	{
		return UniformColor(size, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = (foregroundTexturePtr->GetTextureType() == GRAYSCALE && backgroundTexturePtr->GetTextureType() == GRAYSCALE) ? GRAYSCALE : COLOR;

	TextureMemoryPtr blendedTexturePtr = make_shared<TextureMemory>(textureType, size);
	TextureMemory &blendedTexture = *blendedTexturePtr.get();

	TextureMemory &foregroundTexture = *foregroundTexturePtr.get();
	TextureMemory &backgroundTexture = *backgroundTexturePtr.get();

	if(blendCoefficientTexturePtr.get() == nullptr)
	{
		switch(textureType)
		{
			case GRAYSCALE:
			{
				switch(blendMode)
				{
					//Merge
					case 0:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = min(backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0), 1.0f);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = max(backgroundTexture(i, j, 0) - k * foregroundTexture(i, j, 0), 0.0f);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0) * backgroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * min(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * max(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
				}
				break;
			}
			case COLOR:
			{
				switch(blendMode)
				{
					//Merge
					case 0:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = (1 - k) * backgroundTexture(i, j, 1) + k * foregroundTexture(i, j, 1);
								blendedTexture(i, j, 2) = (1 - k) * backgroundTexture(i, j, 2) + k * foregroundTexture(i, j, 2);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = min(backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0), 1.0f);
								blendedTexture(i, j, 1) = min(backgroundTexture(i, j, 1) + k * foregroundTexture(i, j, 1), 1.0f);
								blendedTexture(i, j, 2) = min(backgroundTexture(i, j, 2) + k * foregroundTexture(i, j, 2), 1.0f);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = max(backgroundTexture(i, j, 0) - k * foregroundTexture(i, j, 0), 0.0f);
								blendedTexture(i, j, 1) = max(backgroundTexture(i, j, 1) - k * foregroundTexture(i, j, 1), 0.0f);
								blendedTexture(i, j, 2) = max(backgroundTexture(i, j, 2) - k * foregroundTexture(i, j, 2), 0.0f);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * foregroundTexture(i, j, 0) * backgroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = (1 - k) * backgroundTexture(i, j, 1) + k * foregroundTexture(i, j, 1) * backgroundTexture(i, j, 1);
								blendedTexture(i, j, 2) = (1 - k) * backgroundTexture(i, j, 2) + k * foregroundTexture(i, j, 2) * backgroundTexture(i, j, 2);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * min(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = (1 - k) * backgroundTexture(i, j, 1) + k * min(foregroundTexture(i, j, 1), backgroundTexture(i, j, 1));
								blendedTexture(i, j, 2) = (1 - k) * backgroundTexture(i, j, 2) + k * min(foregroundTexture(i, j, 2), backgroundTexture(i, j, 2));
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								blendedTexture(i, j, 0) = (1 - k) * backgroundTexture(i, j, 0) + k * max(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = (1 - k) * backgroundTexture(i, j, 1) + k * max(foregroundTexture(i, j, 1), backgroundTexture(i, j, 1));
								blendedTexture(i, j, 2) = (1 - k) * backgroundTexture(i, j, 2) + k * max(foregroundTexture(i, j, 2), backgroundTexture(i, j, 2));
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	else
	{
		TextureMemory &blendCoefficientTexture = *blendCoefficientTexturePtr.get();

		switch(textureType)
		{
			case GRAYSCALE:
			{
				switch(blendMode)
				{
					//Merge
					case 0:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = min(backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0), 1.0f);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = max(backgroundTexture(i, j, 0) - k_ * foregroundTexture(i, j, 0), 0.0f);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0) * backgroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * min(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * max(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
				}
				break;
			}
			case COLOR:
			{
				switch(blendMode)
				{
					//Merge
					case 0:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = (1 - k_) * backgroundTexture(i, j, 1) + k_ * foregroundTexture(i, j, 1);
								blendedTexture(i, j, 2) = (1 - k_) * backgroundTexture(i, j, 2) + k_ * foregroundTexture(i, j, 2);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = min(backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0), 1.0f);
								blendedTexture(i, j, 1) = min(backgroundTexture(i, j, 1) + k_ * foregroundTexture(i, j, 1), 1.0f);
								blendedTexture(i, j, 2) = min(backgroundTexture(i, j, 2) + k_ * foregroundTexture(i, j, 2), 1.0f);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = max(backgroundTexture(i, j, 0) - k_ * foregroundTexture(i, j, 0), 0.0f);
								blendedTexture(i, j, 1) = max(backgroundTexture(i, j, 1) - k_ * foregroundTexture(i, j, 1), 0.0f);
								blendedTexture(i, j, 2) = max(backgroundTexture(i, j, 2) - k_ * foregroundTexture(i, j, 2), 0.0f);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 3);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * foregroundTexture(i, j, 0) * backgroundTexture(i, j, 0);
								blendedTexture(i, j, 1) = (1 - k_) * backgroundTexture(i, j, 1) + k_ * foregroundTexture(i, j, 1) * backgroundTexture(i, j, 1);
								blendedTexture(i, j, 2) = (1 - k_) * backgroundTexture(i, j, 2) + k_ * foregroundTexture(i, j, 2) * backgroundTexture(i, j, 2);
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * min(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = (1 - k_) * backgroundTexture(i, j, 1) + k_ * min(foregroundTexture(i, j, 1), backgroundTexture(i, j, 1));
								blendedTexture(i, j, 2) = (1 - k_) * backgroundTexture(i, j, 2) + k_ * min(foregroundTexture(i, j, 2), backgroundTexture(i, j, 2));
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < size; i++)
						{
							for(int j = 0; j < size; j++)
							{
								float k_ = blendCoefficientTexture(i, j, 0);
								blendedTexture(i, j, 0) = (1 - k_) * backgroundTexture(i, j, 0) + k_ * max(foregroundTexture(i, j, 0), backgroundTexture(i, j, 0));
								blendedTexture(i, j, 1) = (1 - k_) * backgroundTexture(i, j, 1) + k_ * max(foregroundTexture(i, j, 1), backgroundTexture(i, j, 1));
								blendedTexture(i, j, 2) = (1 - k_) * backgroundTexture(i, j, 2) + k_ * max(foregroundTexture(i, j, 2), backgroundTexture(i, j, 2));
								blendedTexture(i, j, 3) = backgroundTexture(i, j, 1);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	

	return blendedTexturePtr;
}


TextureMemoryPtr Levels(TextureMemoryPtr inputTexturePtr, int size, float x1, float x2, float x3, float x4, float x5)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(size, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr leveledTexturePtr = make_shared<TextureMemory>(textureType, size);
	TextureMemory &leveledTexture = *leveledTexturePtr.get();

	TextureMemory &inputTexture = *inputTexturePtr.get();

	float a1 = (-0.5f * x1 + x2 - 0.5f * x3) / ((x2 - x1) * (x3 - x1) * (x3 - x2));
	float b1 = (1 - a1 * (x3 * x3 - x1 * x1)) / (x3 - x1);
	float c1 = -(a1 * x1 * x1 + b1 * x1);

	float a2 = x5 - x4;
	float b2 = x4;

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					float x = inputTexture(i, j, 0);
					leveledTexture(i, j, 0) = a2 * min(max(a1 * x * x + b1 * x + c1, 0.0f), 1.0f) + b2;
					leveledTexture(i, j, 1) = inputTexture(i, j, 1);
				}
			}
			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < size; j++)
				{
					float r = inputTexture(i, j, 0);
					float g = inputTexture(i, j, 1);
					float b = inputTexture(i, j, 2);

					leveledTexture(i, j, 0) = a2 * min(max(a1 * r * r + b1 * r + c1, 0.0f), 1.0f) + b2;
					leveledTexture(i, j, 1) = a2 * min(max(a1 * g * g + b1 * g + c1, 0.0f), 1.0f) + b2;
					leveledTexture(i, j, 2) = a2 * min(max(a1 * b * b + b1 * b + c1, 0.0f), 1.0f) + b2;
					leveledTexture(i, j, 3) = inputTexture(i, j, 3);
				}
			}
			break;
		}
	}

	return leveledTexturePtr;
}


TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, int size)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(size, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr gradientTexturePtr = make_shared<TextureMemory>(COLOR, size);
	TextureMemory &gradientTexture = *gradientTexturePtr.get();

	TextureMemory &inputTexture = *inputTexturePtr.get();

	Grad gradient[72] =
	{
		Grad(145, 97, 77, 0.00f),
		Grad(153, 110, 94, 0.01f),
		Grad(133, 94, 79, 0.02f),
		Grad(142, 107, 91, 0.04f),
		Grad(114, 79, 60, 0.05f),
		Grad(126, 77, 63, 0.09f),
		Grad(116, 61, 41, 0.12f),
		Grad(141, 86, 65, 0.13f),
		Grad(138, 87, 58, 0.15f),
		Grad(193, 139, 111, 0.16f),
		Grad(169, 118, 89, 0.17f),
		Grad(120, 69, 42, 0.20f),
		Grad(161, 111, 84, 0.21f),
		Grad(149, 97, 73, 0.22f),
		Grad(181, 131, 106, 0.25f),
		Grad(152, 102, 77, 0.26f),
		Grad(169, 119, 94, 0.27f),
		Grad(121, 67, 41, 0.28f),
		Grad(89, 46, 27, 0.30f),
		Grad(107, 70, 51, 0.31f),
		Grad(100, 67, 50, 0.32f),
		Grad(122, 93, 77, 0.33f),
		Grad(90, 58, 43, 0.34f),
		Grad(120, 69, 50, 0.35f),
		Grad(102, 44, 24, 0.36f),
		Grad(131, 65, 43, 0.37f),
		Grad(87, 20, 0, 0.38f),
		Grad(94, 30, 3, 0.39f),
		Grad(149, 83, 57, 0.41f),
		Grad(104, 28, 9, 0.42f),
		Grad(123, 46, 26, 0.43f),
		Grad(97, 22, 1, 0.44f),
		Grad(91, 35, 11, 0.46f),
		Grad(119, 64, 40, 0.48f),
		Grad(91, 30, 9, 0.49f),
		Grad(133, 72, 48, 0.50f),
		Grad(109, 52, 22, 0.51f),
		Grad(128, 75, 50, 0.53f),
		Grad(155, 100, 75, 0.54f),
		Grad(147, 90, 63, 0.55f),
		Grad(109, 46, 15, 0.58f),
		Grad(133, 81, 60, 0.62f),
		Grad(116, 61, 40, 0.65f),
		Grad(143, 89, 65, 0.66f),
		Grad(110, 50, 26, 0.69f),
		Grad(142, 81, 56, 0.70f),
		Grad(146, 83, 57, 0.71f),
		Grad(114, 47, 20, 0.72f),
		Grad(155, 85, 59, 0.73f),
		Grad(128, 58, 32, 0.74f),
		Grad(162, 93, 64, 0.75f),
		Grad(137, 68, 39, 0.76f),
		Grad(157, 88, 59, 0.77f),
		Grad(186, 115, 87, 0.78f),
		Grad(181, 110, 82, 0.79f),
		Grad(111, 37, 8, 0.80f),
		Grad(100, 24, 0, 0.81f),
		Grad(145, 69, 43, 0.82f),
		Grad(120, 43, 17, 0.84f),
		Grad(152, 75, 49, 0.85f),
		Grad(122, 44, 21, 0.86f),
		Grad(151, 75, 52, 0.88f),
		Grad(133, 53, 30, 0.89f),
		Grad(170, 88, 64, 0.90f),
		Grad(133, 51, 27, 0.91f),
		Grad(185, 103, 79, 0.92f),
		Grad(166, 84, 60, 0.93f),
		Grad(119, 39, 14, 0.94f),
		Grad(107, 28, 9, 0.95f),
		Grad(87, 9, 0, 0.96f),
		Grad(135, 59, 35, 0.98f),
		Grad(110, 35, 12, 1.00f)
	};

	#pragma omp parallel for
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			int gradientIndex = 0;
			while(gradient[gradientIndex].value < inputTexture(i, j, 0))
			{
				gradientIndex++;
			}

			if(gradientIndex == 0)
			{
				gradientTexture(i, j, 0) = gradient[0].color.x;
				gradientTexture(i, j, 1) = gradient[0].color.y;
				gradientTexture(i, j, 2) = gradient[0].color.z;
				gradientTexture(i, j, 3) = gradient[0].color.w;
			}
			else if(gradientIndex > 71)
			{
				gradientTexture(i, j, 0) = gradient[71].color.x;
				gradientTexture(i, j, 1) = gradient[71].color.y;
				gradientTexture(i, j, 2) = gradient[71].color.z;
				gradientTexture(i, j, 3) = gradient[71].color.w;
			}
			else
			{
				float k = (gradient[gradientIndex].value - inputTexture(i, j, 0)) / (gradient[gradientIndex].value - gradient[gradientIndex - 1].value);

				gradientTexture(i, j, 0) = (1 - k) * gradient[gradientIndex - 1].color.x + k * gradient[gradientIndex].color.x;
				gradientTexture(i, j, 1) = (1 - k) * gradient[gradientIndex - 1].color.y + k * gradient[gradientIndex].color.y;
				gradientTexture(i, j, 2) = (1 - k) * gradient[gradientIndex - 1].color.z + k * gradient[gradientIndex].color.z;
				gradientTexture(i, j, 3) = (1 - k) * gradient[gradientIndex - 1].color.w + k * gradient[gradientIndex].color.w;
			}
		}
	}

	return gradientTexturePtr;
}


TextureMemoryPtr NormalColor(int size)
{
	TextureMemoryPtr normalColorTexturePtr = make_shared<TextureMemory>(COLOR, size);
	TextureMemory &normalColorTexture = *normalColorTexturePtr.get();

	#pragma omp parallel for
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			normalColorTexture(i, j, 0) = 0.5f;
			normalColorTexture(i, j, 1) = 0.5f;
			normalColorTexture(i, j, 2) = 1.0f;
			normalColorTexture(i, j, 3) = 1.0f;
		}
	}

	return normalColorTexturePtr;
}
