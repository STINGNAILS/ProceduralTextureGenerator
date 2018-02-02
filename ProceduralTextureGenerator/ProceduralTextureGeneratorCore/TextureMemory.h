#pragma once

#define TextureMemoryPtr shared_ptr<TextureMemory>


enum TextureType
{
	GRAYSCALE,
	COLOR
};


class TextureMemory
{
	private:

	TextureType textureType;
	int size;
	int offset;

	float *textureMemory;
	
	public:

	TextureMemory();
	TextureMemory(TextureType textureType_, int size_);
	~TextureMemory();

	TextureType GetTextureType();
	int GetSize();

	float &operator()(int u, int v, int channel);
	float &operator[](int i);
};

