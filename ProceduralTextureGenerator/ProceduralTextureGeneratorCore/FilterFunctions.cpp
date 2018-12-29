#include "stdafx.h"
#include "FilterFunctions.h"


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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k) * backgroundSample + k * foregroundSample;
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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = backgroundSample + k * foregroundSample;
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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = backgroundSample - k * foregroundSample;
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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k) * backgroundSample + k * foregroundSample * backgroundSample;
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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k) * backgroundSample + k * min(foregroundSample, backgroundSample);
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
								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k) * backgroundSample + k * max(foregroundSample, backgroundSample);
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k_) * backgroundSample + k_ * foregroundSample;
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = backgroundSample + k_ * foregroundSample;
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = backgroundSample - k_ * foregroundSample;
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k_) * backgroundSample + k_ * foregroundSample * backgroundSample;
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k_) * backgroundSample + k_ * min(foregroundSample, backgroundSample);
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
								float k_ = blendCoefficientTexturePtr->SampleGrayscale(i, j, resolution);

								float backgroundSample = backgroundTexturePtr->SampleGrayscale(i, j, resolution);
								float foregroundSample = foregroundTexturePtr->SampleGrayscale(i, j, resolution);

								float value = (1.0f - k_) * backgroundSample + k_ * max(foregroundSample, backgroundSample);
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


TextureMemoryPtr Remap(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT remapMode, float x1, float y1, float x2, float y2, float x3, float y3)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr remappedTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	float a;
	float b;
	float c;

	switch(remapMode)
	{
		case 1:
		{
			a = 0.0f;
			b = (y1 - y3) / (x1 - x3);
			c = y1 - b * x1;

			break;
		}
		case 2:
		{
			float z1 = x1 * x1 - x3 * x3;
			float z2 = x1 - x3;
			float z3 = x2 * x2 - x3 * x3;
			float z4 = x2 - x3;
			float w1 = y1 - y3;
			float w2 = y2 - y3;

			a = (w1 - w2 * z2 / z4) / (z1 - z3 * z2 / z4);
			b = (w1 - a * z1) / z2;
			c = y1 - b * x1 - a * x1 * x1;

			break;
		}
	}

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					float x = min(max(inputTexturePtr->SampleGrayscale(i, j, resolution), x1), x3);

					float value = c + x * (b + x * a);
					remappedTexturePtr->SetValue(i, j, value);
				}
			}
			break;
		}
		case COLOR:
		{
			XMVECTOR aV = XMVectorReplicate(a);
			XMVECTOR bV = XMVectorReplicate(b);
			XMVECTOR cV = XMVectorReplicate(c);

			XMVECTOR minV = XMVectorReplicate(x1);
			XMVECTOR maxV = XMVectorReplicate(x3);

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMVECTOR xV = XMVectorClamp(XMLoadFloat4(&inputTexturePtr->SampleColor(i, j, resolution)), minV, maxV);

					XMVECTOR valueV = XMVectorMultiplyAdd(XMVectorMultiplyAdd(aV, xV, bV), xV, cV);

					XMFLOAT4 value;
					XMStoreFloat4(&value, valueV);
					remappedTexturePtr->SetValue(i, j, value);
				}
			}
			break;
		}
	}

	return remappedTexturePtr;
}


TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, int gradientSize, vector<float> gradient)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureMemoryPtr gradientTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		for(int j = 0; j < resolution; j++)
		{
			float inputSample = inputTexturePtr->SampleGrayscale(i, j, resolution);

			int gradientIndex = 0;
			for(; gradientIndex < gradientSize; gradientIndex++)
			{
				if(gradient[gradientIndex * 4] >= inputSample)
				{
					break;
				}
			}

			if(gradientIndex == 0)
			{
				gradientTexturePtr->SetValue(i, j, XMFLOAT4(gradient[1], gradient[2], gradient[3], 1.0f));
			}
			else if(gradientIndex >= gradientSize)
			{
				int offset = (gradientSize - 1) * 4;
				gradientTexturePtr->SetValue(i, j, XMFLOAT4(gradient[offset + 1], gradient[offset + 2], gradient[offset + 3], 1.0f));
			}
			else
			{
				int offset1 = (gradientIndex - 1) * 4;
				int offset2 = gradientIndex * 4;

				float k = (inputSample - gradient[offset1]) / (gradient[offset2] - gradient[offset1]);

				XMVECTOR gradientColor1V = XMVectorSet(gradient[offset1 + 1], gradient[offset1 + 2], gradient[offset1 + 3], 1.0f);
				XMVECTOR gradientColor2V = XMVectorSet(gradient[offset2 + 1], gradient[offset2 + 2], gradient[offset2 + 3], 1.0f);

				XMVECTOR valueV = XMVectorLerp(gradientColor1V, gradientColor2V, k);

				XMFLOAT4 value;
				XMStoreFloat4(&value, valueV);
				gradientTexturePtr->SetValue(i, j, value);
			}
		}
	}

	return gradientTexturePtr;
}


TextureMemoryPtr Blur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr horizontallyBlurredTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);
	TextureMemoryPtr blurredTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	int kernelWidth = (int) roundf(resolution / 256.0f * intensity);
	vector<float> kernel(kernelWidth * 2 + 1);

	float factor = 1.0f / sqrt(2 * _Pi * intensity * intensity);
	float norm = 0.0f;
	for(int i = 0; i < kernelWidth; i++)
	{
		kernel[i] = factor * exp(-(kernelWidth - i) * (kernelWidth - i) / (2 * intensity * intensity));
		kernel[kernelWidth * 2 - i] = kernel[i];
		norm += kernel[i] * 2.0f;
	}
	kernel[kernelWidth] = intensity > 0.0f ? factor : 1.0f;
	norm += kernel[kernelWidth];

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					float sum = 0;
					for(int k = 0; k < kernel.size(); k++)
					{
						int index = j + k - kernelWidth;
						index = index >= 0 ? (index < resolution ? index : index - resolution) : index + resolution;

						sum += kernel[k] * inputTexturePtr->SampleGrayscale(i, index, resolution);
					}

					horizontallyBlurredTexturePtr->SetValue(i, j, sum / norm);
				}
			}

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					float sum = 0;
					for(int k = 0; k < kernel.size(); k++)
					{
						int index = i + k - kernelWidth;
						index = index >= 0 ? (index < resolution ? index : index - resolution) : index + resolution;

						sum += kernel[k] * horizontallyBlurredTexturePtr->SampleGrayscale(index, j, resolution);
					}

					blurredTexturePtr->SetValue(i, j, sum / norm);
				}
			}

			break;
		}
		case COLOR:
		{
			XMVECTOR normReciprocalV = XMVectorReplicate(1.0f / norm);

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMVECTOR sumV = XMVectorZero();
					for(int k = 0; k < kernel.size(); k++)
					{
						int index = j + k - kernelWidth;
						index = index >= 0 ? (index < resolution ? index : index - resolution) : index + resolution;

						XMVECTOR kernelV = XMVectorReplicate(kernel[k]);
						XMVECTOR sampleV = XMLoadFloat4(&inputTexturePtr->SampleColor(i, index, resolution));

						sumV = XMVectorMultiplyAdd(kernelV, sampleV, sumV);
					}

					XMFLOAT4 value;
					XMStoreFloat4(&value, XMVectorMultiply(sumV, normReciprocalV));
					horizontallyBlurredTexturePtr->SetValue(i, j, value);
				}
			}

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMVECTOR sumV = XMVectorZero();
					for(int k = 0; k < kernel.size(); k++)
					{
						int index = i + k - kernelWidth;
						index = index >= 0 ? (index < resolution ? index : index - resolution) : index + resolution;

						XMVECTOR kernelV = XMVectorReplicate(kernel[k]);
						XMVECTOR sampleV = XMLoadFloat4(&horizontallyBlurredTexturePtr->SampleColor(index, j, resolution));

						sumV = XMVectorMultiplyAdd(kernelV, sampleV, sumV);
					}

					XMFLOAT4 value;
					XMStoreFloat4(&value, XMVectorMultiply(sumV, normReciprocalV));
					blurredTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
	}

	return blurredTexturePtr;
}



TextureMemoryPtr DirectionalBlur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, float angle)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr blurredTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	int kernelWidth;
	vector<XMFLOAT2> kernel;
	vector<XMINT4> kernelIndices;
	float a = tanf(angle * _Pi / 180.0f);
	float factor = 1.0f / sqrt(2 * _Pi * intensity * intensity);
	float norm = 0.0f;

	if((angle > 45.0f && angle < 135.0f) || (angle > 225.0f && angle < 315.0f))
	{
		kernelWidth = (int) roundf(resolution / 256.0f * intensity * abs(sin(angle * _Pi / 180.0f)));
		kernel.resize(kernelWidth * 2 + 1);
		kernelIndices.resize(kernel.size());

		for(int i = 0; i < kernelWidth; i++)
		{
			float y = i - kernelWidth;
			float x = y / a;

			int xInt = (int) floorf(x);
			int yInt = i - kernelWidth;

			float kernelMidpoint = factor * exp(-(x * x + y * y) / (2 * intensity * intensity));
			kernel[i].x = kernelMidpoint * (xInt + 1 - x);
			kernel[i].y = kernelMidpoint * (x - xInt);
			kernel[kernelWidth * 2 - i].x = kernel[i].y;
			kernel[kernelWidth * 2 - i].y = kernel[i].x;

			kernelIndices[i].x = -yInt;
			kernelIndices[i].y = xInt;
			kernelIndices[i].z = -yInt;
			kernelIndices[i].w = xInt + 1;
			kernelIndices[kernelWidth * 2 - i].x = yInt;
			kernelIndices[kernelWidth * 2 - i].y = -(xInt + 1);
			kernelIndices[kernelWidth * 2 - i].z = yInt;
			kernelIndices[kernelWidth * 2 - i].w = -xInt;

			norm += kernelMidpoint * 2.0f;
		}

		kernel[kernelWidth].x = intensity > 0.0f ? factor * 0.5f : 1.0f;
		kernel[kernelWidth].y = intensity > 0.0f ? factor * 0.5f : 1.0f;

		kernelIndices[kernelWidth].x = 0;
		kernelIndices[kernelWidth].y = 0;
		kernelIndices[kernelWidth].z = 0;
		kernelIndices[kernelWidth].w = 0;

		norm += factor;
	}
	else
	{
		kernelWidth = (int) roundf(resolution / 256.0f * intensity *  abs(cos(angle * _Pi / 180.0f)));
		kernel.resize(kernelWidth * 2 + 1);
		kernelIndices.resize(kernel.size());

		for(int i = 0; i < kernelWidth; i++)
		{
			float x = i - kernelWidth;
			float y = a * x;

			int xInt = i - kernelWidth;
			int yInt = (int) ceilf(y);

			float kernelMidpoint = factor * exp(-(x * x + y * y) / (2 * intensity * intensity));
			kernel[i].x = kernelMidpoint * (y + 1 - yInt);
			kernel[i].y = kernelMidpoint * (yInt - y);
			kernel[kernelWidth * 2 - i].x = kernel[i].y;
			kernel[kernelWidth * 2 - i].y = kernel[i].x;

			kernelIndices[i].x = -yInt;
			kernelIndices[i].y = xInt;
			kernelIndices[i].z = -(yInt - 1);
			kernelIndices[i].w = xInt;
			kernelIndices[kernelWidth * 2 - i].x = yInt - 1;
			kernelIndices[kernelWidth * 2 - i].y = -xInt;
			kernelIndices[kernelWidth * 2 - i].z = yInt;
			kernelIndices[kernelWidth * 2 - i].w = -xInt;

			norm += kernelMidpoint * 2.0f;
		}

		kernel[kernelWidth].x = intensity > 0.0f ? factor * 0.5f : 1.0f;
		kernel[kernelWidth].y = intensity > 0.0f ? factor * 0.5f : 1.0f;

		kernelIndices[kernelWidth].x = 0;
		kernelIndices[kernelWidth].y = 0;
		kernelIndices[kernelWidth].z = 0;
		kernelIndices[kernelWidth].w = 0;

		norm += factor;
	}

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					float sum = 0;
					for(int k = 0; k < kernel.size(); k++)
					{
						int i0 = i + kernelIndices[k].x;
						int j0 = j + kernelIndices[k].y;
						int i1 = i + kernelIndices[k].z;
						int j1 = j + kernelIndices[k].w;

						i0 = i0 >= 0 ? (i0 < resolution ? i0 : i0 - resolution) : i0 + resolution;
						j0 = j0 >= 0 ? (j0 < resolution ? j0 : j0 - resolution) : j0 + resolution;
						i1 = i1 >= 0 ? (i1 < resolution ? i1 : i1 - resolution) : i1 + resolution;
						j1 = j1 >= 0 ? (j1 < resolution ? j1 : j1 - resolution) : j1 + resolution;

						sum += kernel[k].x * inputTexturePtr->SampleGrayscale(i0, j0, resolution) + kernel[k].y * inputTexturePtr->SampleGrayscale(i1, j1, resolution);
					}

					blurredTexturePtr->SetValue(i, j, sum / norm);
				}
			}

			break;
		}
		case COLOR:
		{
			XMVECTOR normReciprocalV = XMVectorReplicate(1.0f / norm);

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					XMVECTOR sumV = XMVectorZero();
					for(int k = 0; k < kernel.size(); k++)
					{
						int i0 = i + kernelIndices[k].x;
						int j0 = j + kernelIndices[k].y;
						int i1 = i + kernelIndices[k].z;
						int j1 = j + kernelIndices[k].w;

						i0 = i0 >= 0 ? (i0 < resolution ? i0 : i0 - resolution) : i0 + resolution;
						j0 = j0 >= 0 ? (j0 < resolution ? j0 : j0 - resolution) : j0 + resolution;
						i1 = i1 >= 0 ? (i1 < resolution ? i1 : i1 - resolution) : i1 + resolution;
						j1 = j1 >= 0 ? (j1 < resolution ? j1 : j1 - resolution) : j1 + resolution;

						XMVECTOR kernel0V = XMVectorReplicate(kernel[k].x);
						XMVECTOR kernel1V = XMVectorReplicate(kernel[k].y);
						XMVECTOR sample0V = XMLoadFloat4(&inputTexturePtr->SampleColor(i0, j0, resolution));
						XMVECTOR sample1V = XMLoadFloat4(&inputTexturePtr->SampleColor(i1, j1, resolution));

						sumV = XMVectorAdd(XMVectorMultiplyAdd(kernel0V, sample0V, XMVectorMultiply(kernel1V, sample1V)), sumV);
					}

					XMFLOAT4 value;
					XMStoreFloat4(&value, XMVectorMultiply(sumV, normReciprocalV));
					blurredTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
	}

	return blurredTexturePtr;
}


TextureMemoryPtr Transform(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, int tilingMode, float xScale, float yScale, float rotation, float xTranslate, float yTranslate)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr transformedTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	double s = sin(rotation * _Pi / 180.0);
	double c = cos(rotation * _Pi / 180.0);

	switch(textureType)
	{
		case GRAYSCALE:
		{
			switch(tilingMode)
			{
				//Wrap
				case 0:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						double y = 0.5 - (double) i / (resolution - 1);
						for(int j = 0; j < resolution; j++)
						{
							double x = (double) j / (resolution - 1) - 0.5;

							double x_ = (c * (x - xTranslate) + s * (y - yTranslate)) / xScale;
							double y_ = (-s * (x - xTranslate) + c * (y - yTranslate)) / yScale;

							x_ -= floor(x_ + 0.5);
							y_ -= floor(y_ + 0.5);

							int i0 = (int) floor((0.5 - y_) * (resolution - 1));
							int i1 = i0 + 1;
							int j0 = (int) floor((x_ + 0.5) * (resolution - 1));
							int j1 = j0 + 1;
							i0 = (i0 >= 0) ? (i0 < resolution ? i0 : (i0 - resolution)) : (resolution + i0);
							i1 = (i1 >= 0) ? (i1 < resolution ? i1 : (i1 - resolution)) : (resolution + i1);
							j0 = (j0 >= 0) ? (j0 < resolution ? j0 : (j0 - resolution)) : (resolution + j0);
							j1 = (j1 >= 0) ? (j1 < resolution ? j1 : (j1 - resolution)) : (resolution + j1);

							double x0 = (double) j0 / (resolution - 1) - 0.5;
							double x1 = (double) j1 / (resolution - 1) - 0.5;
							double y0 = 0.5 - (double) i0 / (resolution - 1);
							double y1 = 0.5 - (double) i1 / (resolution - 1);

							double kx = (x_ - x0) / (x1 - x0);
							double ky = (y_ - y0) / (y1 - y0);

							float value00 = inputTexturePtr->SampleGrayscale(i0, j0, resolution);
							float value01 = inputTexturePtr->SampleGrayscale(i0, j1, resolution);
							float value10 = inputTexturePtr->SampleGrayscale(i1, j0, resolution);
							float value11 = inputTexturePtr->SampleGrayscale(i1, j1, resolution);

							transformedTexturePtr->SetValue(i, j, (1.0 - kx) * (1.0 - ky) * value00 + kx * (1.0 - ky) * value01 + (1.0 - kx) * ky * value10 + kx * ky * value11);
						}
					}

					break;
				}
				//Clamp
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						double y = 0.5 - (double) i / (resolution - 1);
						for(int j = 0; j < resolution; j++)
						{
							double x = (double) j / (resolution - 1) - 0.5;

							double x_ = (c * (x - xTranslate) + s * (y - yTranslate)) / xScale;
							double y_ = (-s * (x - xTranslate) + c * (y - yTranslate)) / yScale;

							if(x_ < -0.5 || x_ > 0.5 || y_ < -0.5 || y_ > 0.5)
							{
								transformedTexturePtr->SetValue(i, j, 0.0f);
							}
							else
							{
								int i0 = (int) floor((0.5 - y_) * (resolution - 1));
								int i1 = i0 + 1;
								int j0 = (int) floor((x_ + 0.5) * (resolution - 1));
								int j1 = j0 + 1;
								i0 = (i0 >= 0) ? (i0 < resolution ? i0 : (i0 - resolution)) : (resolution + i0);
								i1 = (i1 >= 0) ? (i1 < resolution ? i1 : (i1 - resolution)) : (resolution + i1);
								j0 = (j0 >= 0) ? (j0 < resolution ? j0 : (j0 - resolution)) : (resolution + j0);
								j1 = (j1 >= 0) ? (j1 < resolution ? j1 : (j1 - resolution)) : (resolution + j1);

								double x0 = (double) j0 / (resolution - 1) - 0.5;
								double x1 = (double) j1 / (resolution - 1) - 0.5;
								double y0 = 0.5 - (double) i0 / (resolution - 1);
								double y1 = 0.5 - (double) i1 / (resolution - 1);

								double kx = (x_ - x0) / (x1 - x0);
								double ky = (y_ - y0) / (y1 - y0);

								float value00 = inputTexturePtr->SampleGrayscale(i0, j0, resolution);
								float value01 = inputTexturePtr->SampleGrayscale(i0, j1, resolution);
								float value10 = inputTexturePtr->SampleGrayscale(i1, j0, resolution);
								float value11 = inputTexturePtr->SampleGrayscale(i1, j1, resolution);

								transformedTexturePtr->SetValue(i, j, (1.0 - kx) * (1.0 - ky) * value00 + kx * (1.0 - ky) * value01 + (1.0 - kx) * ky * value10 + kx * ky * value11);
							}
						}
					}

					break;
				}
			}

			break;
		}
		case COLOR:
		{
			switch(tilingMode)
			{
				//Wrap
				case 0:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						double y = 0.5 - (double) i / (resolution - 1);
						for(int j = 0; j < resolution; j++)
						{
							double x = (double) j / (resolution - 1) - 0.5;

							double x_ = (c * (x - xTranslate) + s * (y - yTranslate)) / xScale;
							double y_ = (-s * (x - xTranslate) + c * (y - yTranslate)) / yScale;

							x_ -= floor(x_ + 0.5);
							y_ -= floor(y_ + 0.5);

							int i0 = (int) floor((0.5 - y_) * (resolution - 1));
							int i1 = i0 + 1;
							int j0 = (int) floor((x_ + 0.5) * (resolution - 1));
							int j1 = j0 + 1;
							i0 = (i0 >= 0) ? (i0 < resolution ? i0 : (i0 - resolution)) : (resolution + i0);
							i1 = (i1 >= 0) ? (i1 < resolution ? i1 : (i1 - resolution)) : (resolution + i1);
							j0 = (j0 >= 0) ? (j0 < resolution ? j0 : (j0 - resolution)) : (resolution + j0);
							j1 = (j1 >= 0) ? (j1 < resolution ? j1 : (j1 - resolution)) : (resolution + j1);

							double x0 = (double) j0 / (resolution - 1) - 0.5;
							double x1 = (double) j1 / (resolution - 1) - 0.5;
							double y0 = 0.5 - (double) i0 / (resolution - 1);
							double y1 = 0.5 - (double) i1 / (resolution - 1);

							double kx = (x_ - x0) / (x1 - x0);
							double ky = (y_ - y0) / (y1 - y0);

							XMVECTOR value00V = XMLoadFloat4(&inputTexturePtr->SampleColor(i0, j0, resolution));
							XMVECTOR value01V = XMLoadFloat4(&inputTexturePtr->SampleColor(i0, j1, resolution));
							XMVECTOR value10V = XMLoadFloat4(&inputTexturePtr->SampleColor(i1, j0, resolution));
							XMVECTOR value11V = XMLoadFloat4(&inputTexturePtr->SampleColor(i1, j1, resolution));

							XMVECTOR value0V = XMVectorLerp(value00V, value01V, kx);
							XMVECTOR value1V = XMVectorLerp(value10V, value11V, kx);

							XMVECTOR valueV = XMVectorLerp(value0V, value1V, ky);

							XMFLOAT4 value;
							XMStoreFloat4(&value, valueV);
							transformedTexturePtr->SetValue(i, j, value);
						}
					}

					break;
				}
				//Clamp
				case 1:
				{
					#pragma omp parallel for
					for(int i = 0; i < resolution; i++)
					{
						double y = 0.5 - (double) i / (resolution - 1);
						for(int j = 0; j < resolution; j++)
						{
							double x = (double) j / (resolution - 1) - 0.5;

							double x_ = (c * (x - xTranslate) + s * (y - yTranslate)) / xScale;
							double y_ = (-s * (x - xTranslate) + c * (y - yTranslate)) / yScale;

							if(x_ < -0.5 || x_ > 0.5 || y_ < -0.5 || y_ > 0.5)
							{
								transformedTexturePtr->SetValue(i, j, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
							}
							else
							{
								int i0 = (int) floor((0.5 - y_) * (resolution - 1));
								int i1 = i0 + 1;
								int j0 = (int) floor((x_ + 0.5) * (resolution - 1));
								int j1 = j0 + 1;
								i0 = (i0 >= 0) ? (i0 < resolution ? i0 : (i0 - resolution)) : (resolution + i0);
								i1 = (i1 >= 0) ? (i1 < resolution ? i1 : (i1 - resolution)) : (resolution + i1);
								j0 = (j0 >= 0) ? (j0 < resolution ? j0 : (j0 - resolution)) : (resolution + j0);
								j1 = (j1 >= 0) ? (j1 < resolution ? j1 : (j1 - resolution)) : (resolution + j1);

								double x0 = (double) j0 / (resolution - 1) - 0.5;
								double x1 = (double) j1 / (resolution - 1) - 0.5;
								double y0 = 0.5 - (double) i0 / (resolution - 1);
								double y1 = 0.5 - (double) i1 / (resolution - 1);

								double kx = (x_ - x0) / (x1 - x0);
								double ky = (y_ - y0) / (y1 - y0);

								XMVECTOR value00V = XMLoadFloat4(&inputTexturePtr->SampleColor(i0, j0, resolution));
								XMVECTOR value01V = XMLoadFloat4(&inputTexturePtr->SampleColor(i0, j1, resolution));
								XMVECTOR value10V = XMLoadFloat4(&inputTexturePtr->SampleColor(i1, j0, resolution));
								XMVECTOR value11V = XMLoadFloat4(&inputTexturePtr->SampleColor(i1, j1, resolution));

								XMVECTOR value0V = XMVectorLerp(value00V, value01V, kx);
								XMVECTOR value1V = XMVectorLerp(value10V, value11V, kx);

								XMVECTOR valueV = XMVectorLerp(value0V, value1V, ky);

								XMFLOAT4 value;
								XMStoreFloat4(&value, valueV);
								transformedTexturePtr->SetValue(i, j, value);
							}
						}
					}

					break;
				}
			}

			break;
		}
	}

	return transformedTexturePtr;
}


TextureMemoryPtr Warp(TextureMemoryPtr inputTexturePtr, TextureMemoryPtr slopeTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity)
{
	if(inputTexturePtr.get() == nullptr || slopeTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr warpedTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	float factor = intensity * resolution / 256.0f;

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				int i1 = (i == 0) ? (resolution - 1) : (i - 1);
				float u = (float) i / (resolution - 1);
				for(int j = 0; j < resolution; j++)
				{
					int j1 = (j == 0) ? (resolution - 1) : (j - 1);
					float v = (float) j / (resolution - 1);

					float slopeXY0 = slopeTexturePtr->SampleGrayscale(i, j, resolution);
					float slopeX1 = slopeTexturePtr->SampleGrayscale(i, j1, resolution);
					float slopeY1 = slopeTexturePtr->SampleGrayscale(i1, j, resolution);

					float uWarped = u + (slopeXY0 - slopeY1) * factor;
					float vWarped = v + (slopeXY0 - slopeX1) * factor;

					float uWarpedNormalized = uWarped - floor(uWarped);
					float vWarpedNormalized = vWarped - floor(vWarped);

					int iWarped = (int) floor(uWarpedNormalized * (resolution - 1));
					int jWarped = (int) floor(vWarpedNormalized * (resolution - 1));

					float value = inputTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);

					warpedTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				int i1 = (i == 0) ? (resolution - 1) : (i - 1);
				float u = (float) i / (resolution - 1);
				for(int j = 0; j < resolution; j++)
				{
					int j1 = (j == 0) ? (resolution - 1) : (j - 1);
					float v = (float) j / (resolution - 1);

					float slopeXY0 = slopeTexturePtr->SampleGrayscale(i, j, resolution);
					float slopeX1 = slopeTexturePtr->SampleGrayscale(i, j1, resolution);
					float slopeY1 = slopeTexturePtr->SampleGrayscale(i1, j, resolution);

					float uWarped = u + (slopeXY0 - slopeY1) * factor;
					float vWarped = v + (slopeXY0 - slopeX1) * factor;

					float uWarpedNormalized = uWarped - floor(uWarped);
					float vWarpedNormalized = vWarped - floor(vWarped);

					int iWarped = (int) floor(uWarpedNormalized * (resolution - 1));
					int jWarped = (int) floor(vWarpedNormalized * (resolution - 1));

					XMFLOAT4 value = inputTexturePtr->SampleColor(iWarped, jWarped, resolution);

					warpedTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
	}

	return warpedTexturePtr;
}


TextureMemoryPtr SlopeBlur(TextureMemoryPtr inputTexturePtr, TextureMemoryPtr slopeTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, int samplesNum)
{
	if(inputTexturePtr.get() == nullptr || slopeTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr blurredTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	float intensityWar = (samplesNum != 0) ? (intensity / samplesNum) : 0.0f;
	float factor = intensityWar * resolution / 256.0f;

	switch(textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				int i1 = (i == 0) ? (resolution - 1) : (i - 1);
				float u = (float) i / (resolution - 1);
				for(int j = 0; j < resolution; j++)
				{
					int j1 = (j == 0) ? (resolution - 1) : (j - 1);
					float v = (float) j / (resolution - 1);

					float slopeXY0 = slopeTexturePtr->SampleGrayscale(i, j, resolution);
					float slopeX1 = slopeTexturePtr->SampleGrayscale(i, j1, resolution);
					float slopeY1 = slopeTexturePtr->SampleGrayscale(i1, j, resolution);

					float uWarped = u + (slopeXY0 - slopeY1) * factor;
					float vWarped = v + (slopeXY0 - slopeX1) * factor;

					uWarped -= floor(uWarped);
					vWarped -= floor(vWarped);

					int iWarped = (int) floor(uWarped * (resolution - 1));
					int jWarped = (int) floor(vWarped * (resolution - 1));

					float value = inputTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);

					for(int k = 1; k < samplesNum; k++)
					{
						int iWarped1 = (iWarped == 0) ? (resolution - 1) : (iWarped - 1);
						int jWarped1 = (jWarped == 0) ? (resolution - 1) : (jWarped - 1);

						slopeXY0 = slopeTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);
						slopeX1 = slopeTexturePtr->SampleGrayscale(iWarped, jWarped1, resolution);
						slopeY1 = slopeTexturePtr->SampleGrayscale(iWarped1, jWarped, resolution);

						uWarped += (slopeXY0 - slopeY1) * factor;
						vWarped += (slopeXY0 - slopeX1) * factor;

						uWarped -= floor(uWarped);
						vWarped -= floor(vWarped);

						iWarped = (int) floor(uWarped * (resolution - 1));
						jWarped = (int) floor(vWarped * (resolution - 1));

						float blendFactor = 1.0f / (1.0f + k);

						value = (1.0f - blendFactor) * value + blendFactor * inputTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);
					}

					blurredTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				int i1 = (i == 0) ? (resolution - 1) : (i - 1);
				float u = (float) i / (resolution - 1);
				for(int j = 0; j < resolution; j++)
				{
					int j1 = (j == 0) ? (resolution - 1) : (j - 1);
					float v = (float) j / (resolution - 1);

					float slopeXY0 = slopeTexturePtr->SampleGrayscale(i, j, resolution);
					float slopeX1 = slopeTexturePtr->SampleGrayscale(i, j1, resolution);
					float slopeY1 = slopeTexturePtr->SampleGrayscale(i1, j, resolution);

					float uWarped = u + (slopeXY0 - slopeY1) * factor;
					float vWarped = v + (slopeXY0 - slopeX1) * factor;

					uWarped -= floor(uWarped);
					vWarped -= floor(vWarped);

					int iWarped = (int) floor(uWarped * (resolution - 1));
					int jWarped = (int) floor(vWarped * (resolution - 1));

					XMVECTOR valueV = XMLoadFloat4(&inputTexturePtr->SampleColor(iWarped, jWarped, resolution));

					for(int k = 1; k < samplesNum; k++)
					{
						int iWarped1 = (iWarped == 0) ? (resolution - 1) : (iWarped - 1);
						int jWarped1 = (jWarped == 0) ? (resolution - 1) : (jWarped - 1);

						slopeXY0 = slopeTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);
						slopeX1 = slopeTexturePtr->SampleGrayscale(iWarped, jWarped1, resolution);
						slopeY1 = slopeTexturePtr->SampleGrayscale(iWarped1, jWarped, resolution);

						uWarped += (slopeXY0 - slopeY1) * factor;
						vWarped += (slopeXY0 - slopeX1) * factor;

						uWarped -= floor(uWarped);
						vWarped -= floor(vWarped);

						iWarped = (int) floor(uWarped * (resolution - 1));
						jWarped = (int) floor(vWarped * (resolution - 1));

						float blendFactor = 1.0f / (1.0f + k);

						XMVECTOR value1V = XMLoadFloat4(&inputTexturePtr->SampleColor(iWarped, jWarped, resolution));
						valueV = XMVectorLerp(valueV, value1V, blendFactor);
					}

					XMFLOAT4 value;
					XMStoreFloat4(&value, valueV);
					blurredTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
	}

	return blurredTexturePtr;
}


TextureMemoryPtr DirectionalWarp(TextureMemoryPtr inputTexturePtr, TextureMemoryPtr intensityTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, float angle)
{
	if (inputTexturePtr.get() == nullptr || intensityTexturePtr.get() == nullptr)
	{
		return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	TextureType textureType = inputTexturePtr->GetTextureType();

	TextureMemoryPtr warpedTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	float baseSampleLength = intensity / 256.0f;
	float duBase = baseSampleLength * cos(angle * _Pi / 180.0f);
	float dvBase = baseSampleLength * sin(angle * _Pi / 180.0f);
	
	switch (textureType)
	{
		case GRAYSCALE:
		{
			#pragma omp parallel for
			for (int i = 0; i < resolution; i++)
			{
				float u = (float)i / (resolution - 1);
				for (int j = 0; j < resolution; j++)
				{
					float v = (float)j / (resolution - 1);

					float localIntensity = intensityTexturePtr->SampleGrayscale(i, j, resolution);

					float uWarped = u + duBase * localIntensity;
					float vWarped = v + dvBase * localIntensity;

					float uWarpedNormalized = uWarped - floor(uWarped);
					float vWarpedNormalized = vWarped - floor(vWarped);

					int iWarped = (int)floor(uWarpedNormalized * (resolution - 1));
					int jWarped = (int)floor(vWarpedNormalized * (resolution - 1));

					float value = inputTexturePtr->SampleGrayscale(iWarped, jWarped, resolution);

					warpedTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for (int i = 0; i < resolution; i++)
			{
				float u = (float)i / (resolution - 1);
				for (int j = 0; j < resolution; j++)
				{
					float v = (float)j / (resolution - 1);

					float localIntensity = intensityTexturePtr->SampleGrayscale(i, j, resolution);

					float uWarped = u + duBase * localIntensity;
					float vWarped = v + dvBase * localIntensity;

					float uWarpedNormalized = uWarped - floor(uWarped);
					float vWarpedNormalized = vWarped - floor(vWarped);

					int iWarped = (int)floor(uWarpedNormalized * (resolution - 1));
					int jWarped = (int)floor(vWarpedNormalized * (resolution - 1));

					XMVECTOR valueV = XMLoadFloat4(&inputTexturePtr->SampleColor(iWarped, jWarped, resolution));

					XMFLOAT4 value;
					XMStoreFloat4(&value, valueV);
					warpedTexturePtr->SetValue(i, j, value);
				}
			}

			break;
		}
	}

	return warpedTexturePtr;
}
