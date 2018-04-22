#include "stdafx.h"
#include "NormalFunctions.h"


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


TextureMemoryPtr HeightToNormal(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float height)
{
	if(inputTexturePtr.get() == nullptr)
	{
		return NormalColor(resolution, bitsPerChannel);
	}

	TextureMemoryPtr normalTexturePtr = make_shared<TextureMemory>(COLOR, resolution, bitsPerChannel);

	float step = 2.0f / resolution;
	XMVECTOR bias = XMVectorReplicate(0.5f);

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		int i1 = i != 0 ? i - 1 : resolution - 1;
		int i2 = i != resolution - 1 ? i + 1 : 0;

		for(int j = 0; j < resolution; j++)
		{
			int j1 = j != 0 ? j - 1 : resolution - 1;
			int j2 = j != resolution - 1 ? j + 1 : 0;

			XMVECTOR tangentV = XMVectorSet(step, 0.0f, height * (inputTexturePtr->SampleGrayscale(i, j2, resolution).x - inputTexturePtr->SampleGrayscale(i, j1, resolution).x), 0.0f);
			XMVECTOR binormalV = XMVectorSet(0.0f, step, height * (inputTexturePtr->SampleGrayscale(i1, j, resolution).x - inputTexturePtr->SampleGrayscale(i2, j, resolution).x), 0.0f);

			XMVECTOR normalV = XMVector3Normalize(XMVector3Cross(tangentV, binormalV));
			XMVECTOR valueV = XMVectorMultiplyAdd(bias, normalV, bias);

			XMFLOAT4 value;
			XMStoreFloat4(&value, valueV);
			normalTexturePtr->SetValue(i, j, value);
		}
	}

	return normalTexturePtr;
}