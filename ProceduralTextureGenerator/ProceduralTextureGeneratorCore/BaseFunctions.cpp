#include "stdafx.h"
#include "BaseFunctions.h"


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