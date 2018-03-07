#include "stdafx.h"
#include "AtomicFunctions.h"


TextureMemoryPtr BaseColor(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr baseColorTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);
	
	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			baseColorTexturePtr->SetValue(i, j, inputTexturePtr->SampleColor(i, j, resolution));
		}
	}

	return baseColorTexturePtr;
}


TextureMemoryPtr Metallic(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, GRAYSCALE, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr metallicTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			metallicTexturePtr->SetValue(i, j, inputTexturePtr->SampleGrayscale(i, j, resolution));
		}
	}

	return metallicTexturePtr;
}


TextureMemoryPtr Roughness(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, GRAYSCALE, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr roughnessTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			roughnessTexturePtr->SetValue(i, j, inputTexturePtr->SampleGrayscale(i, j, resolution));
		}
	}

	return roughnessTexturePtr;
}


TextureMemoryPtr Normal(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return NormalColor(resolution, bitsPerChannel);
	}

	TextureMemoryPtr normalTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			normalTexturePtr->SetValue(i, j, inputTexturePtr->SampleColor(i, j, resolution));
		}
	}

	return normalTexturePtr;
}


TextureMemoryPtr Height(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, GRAYSCALE, XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	TextureMemoryPtr heightTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			heightTexturePtr->SetValue(i, j, inputTexturePtr->SampleGrayscale(i, j, resolution));
		}
	}

	return heightTexturePtr;
}


TextureMemoryPtr UniformColor(TextureResolution resolution, BitsPerChannel bitsPerChannel, TextureType textureType, XMFLOAT4 color)
{
	TextureMemoryPtr uniformColorTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	switch(textureType)
	{
		case GRAYSCALE:
		{
			XMFLOAT2 color_ = XMFLOAT2(color.x, color.w);

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					uniformColorTexturePtr->SetValue(i, j, color_);
				}
			}
			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					uniformColorTexturePtr->SetValue(i, j, color);
				}
			}
			break;
		}
	}

	return uniformColorTexturePtr;
}


TextureMemoryPtr Blend(TextureMemoryPtr foregroundTexturePtr, TextureMemoryPtr backgroundTexturePtr, TextureMemoryPtr blendCoefficientTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT blendMode, float k)
{
	if(foregroundTexturePtr.get() == nullptr || backgroundTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = (foregroundTexturePtr->GetTextureType() == GRAYSCALE && backgroundTexturePtr->GetTextureType() == GRAYSCALE) ? GRAYSCALE : COLOR;

	TextureMemoryPtr blendedTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

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
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k) * backgroundSample.x + k * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2(backgroundSample.x + k * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2(backgroundSample.x - k * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k) * backgroundSample.x + k * foregroundSample.x * backgroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k) * backgroundSample.x + k * min(foregroundSample.x, backgroundSample.x), 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k) * backgroundSample.x + k * max(foregroundSample.x, backgroundSample.x), 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
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
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerp(backgroundSampleV, foregroundSampleV, k);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR kV = XMVectorReplicate(k);
								XMVECTOR valueV = XMVectorMultiplyAdd(foregroundSampleV, kV, backgroundSampleV);
								
								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR kV = XMVectorReplicate(-k);
								XMVECTOR valueV = XMVectorMultiplyAdd(foregroundSampleV, kV, backgroundSampleV);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerp(backgroundSampleV, XMVectorMultiply(foregroundSampleV, backgroundSampleV), k);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerp(backgroundSampleV, XMVectorMin(foregroundSampleV, backgroundSampleV), k);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerp(backgroundSampleV, XMVectorMax(foregroundSampleV, backgroundSampleV), k);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
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
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k_) * backgroundSample.x + k_ * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2(backgroundSample.x + k_ * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2(backgroundSample.x - k_ * foregroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k_) * backgroundSample.x + k_ * foregroundSample.x * backgroundSample.x, 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k_) * backgroundSample.x + k_ * min(foregroundSample.x, backgroundSample.x), 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution).x;

								XMFLOAT2 backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								XMFLOAT2 foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								XMFLOAT2 value = XMFLOAT2((1.0f - k_) * backgroundSample.x + k_ * max(foregroundSample.x, backgroundSample.x), 1.0f);
								blendedTexturePtr->SetValue(i, j, value);
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
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerpV(backgroundSampleV, foregroundSampleV, blendCoefficientSampleV);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Add
					case 1:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorMultiplyAdd(foregroundSampleV, blendCoefficientSampleV, backgroundSampleV);
								
								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Sub
					case 2:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorMultiplyAdd(foregroundSampleV, XMVectorNegate(blendCoefficientSampleV), backgroundSampleV);
								
								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Multiply
					case 3:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerpV(backgroundSampleV, XMVectorMultiply(foregroundSampleV, backgroundSampleV), blendCoefficientSampleV);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Min
					case 4:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerpV(backgroundSampleV, XMVectorMin(foregroundSampleV, backgroundSampleV), blendCoefficientSampleV);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
							}
						}
						break;
					}
					//Max
					case 5:
					{
						#pragma omp parallel for
						for(int i = 0; i < resolution; i++)
						{
							for(int j = 0; j < resolution; j++)
							{
								XMVECTOR backgroundSampleV = XMLoadFloat4(&backgroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR foregroundSampleV = XMLoadFloat4(&foregroundTexturePtr->SampleColor(i, j, resolution));
								XMVECTOR blendCoefficientSampleV = XMLoadFloat4(&blendCoefficientTexturePtr->SampleColor(i, j, resolution));

								XMVECTOR valueV = XMVectorLerpV(backgroundSampleV, XMVectorMax(foregroundSampleV, backgroundSampleV), blendCoefficientSampleV);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								blendedTexturePtr->SetValue(i, j, value);
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


TextureMemoryPtr Levels(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float x1, float x2, float x3, float x4, float x5)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr leveledTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

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
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					float x = inputTexturePtr->SampleGrayscale(i, j, resolution).x;

					XMFLOAT2 value = XMFLOAT2(a2 * min(max(a1 * x * x + b1 * x + c1, 0.0f), 1.0f) + b2, 1.0f);
					leveledTexturePtr->SetValue(i, j, value);
				}
			}
			break;
		}
		case COLOR:
		{
			XMVECTOR a1V = XMVectorReplicate(a1);
			XMVECTOR b1V = XMVectorReplicate(b1);
			XMVECTOR c1V = XMVectorReplicate(c1);

			XMVECTOR a2V = XMVectorReplicate(a2);
			XMVECTOR b2V = XMVectorReplicate(b2);

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMVECTOR inputSampleV = XMLoadFloat4(&inputTexturePtr->SampleColor(i, j, resolution));

					XMVECTOR valueV = XMVectorMultiplyAdd(a2V, XMVectorSaturate(XMVectorMultiplyAdd(XMVectorMultiplyAdd(a1V, inputSampleV, b1V), inputSampleV, c1V)), b2V);

					XMFLOAT4 value;
					XMStoreFloat4(&value, valueV);
					leveledTexturePtr->SetValue(i, j, value);
				}
			}
			break;
		}
	}

	return leveledTexturePtr;
}


TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr gradientTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);

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
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float inputSample = inputTexturePtr->SampleGrayscale(i, j, resolution).x;

			int gradientIndex = 0;
			while(gradient[gradientIndex].value < inputSample)
			{
				gradientIndex++;
			}

			if(gradientIndex == 0)
			{
				gradientTexturePtr->SetValue(i, j, gradient[0].color);
			}
			else if(gradientIndex > 71)
			{
				gradientTexturePtr->SetValue(i, j, gradient[71].color);
			}
			else
			{
				float k = (gradient[gradientIndex].value - inputSample) / (gradient[gradientIndex].value - gradient[gradientIndex - 1].value);

				XMVECTOR gradientColor1V = XMLoadFloat4(&gradient[gradientIndex - 1].color);
				XMVECTOR gradientColor2V = XMLoadFloat4(&gradient[gradientIndex].color);

				XMVECTOR valueV = XMVectorLerp(gradientColor1V, gradientColor2V, k);

				XMFLOAT4 value;
				XMStoreFloat4(&value, valueV);
				gradientTexturePtr->SetValue(i, j, value);
			}
		}
	}

	return gradientTexturePtr;
}


TextureMemoryPtr NormalColor(TextureResolution resolution, BitsPerChannel bitsPerChannel)
{
	TextureMemoryPtr normalColorTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			normalColorTexturePtr->SetValue(i, j, XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f));
		}
	}

	return normalColorTexturePtr;
}
