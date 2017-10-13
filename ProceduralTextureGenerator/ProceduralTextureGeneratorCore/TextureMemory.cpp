#include "stdafx.h"
#include "TextureMemory.h"


TextureMemory::TextureMemory()
{
	
}


TextureMemory::TextureMemory(TextureType textureType_, int size_)
{
	textureType = textureType_;
	size = size_;
	offset = textureType == GRAYSCALE ? 2 : 4;

	textureMemory = new float[size * size * offset];
	ZeroMemory(textureMemory, size * size * offset * sizeof(float));
}


TextureMemory::~TextureMemory()
{
	delete[] textureMemory;
}


TextureType TextureMemory::GetTextureType()
{
	return textureType;
}


int TextureMemory::GetSize()
{
	return size;
}


float &TextureMemory::operator()(int u, int v, int channel)
{
	return textureMemory[(size * u + v) * offset + channel];
}


float &TextureMemory::operator[](int i)
{
	return textureMemory[i];
}
