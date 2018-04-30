#pragma once

#include "DirectXDevice.h"


#define TextureMemoryPtr shared_ptr<TextureMemory>


enum TextureType
{
	GRAYSCALE = 0,
	COLOR     = 1
};


enum TextureResolution
{
	RES16   = 16,
	RES32   = 32,
	RES64   = 64,
	RES128  = 128,
	RES256  = 256,
	RES512  = 512,
	RES1024 = 1024,
	RES2048 = 2048,
	RES4096 = 4096
};


enum BitsPerChannel
{
	BPC8  = 8,
	BPC16 = 16,
	BPC32 = 32
};


class TextureMemory
{
	private:

	TextureType textureType;
	TextureResolution textureResolution;
	BitsPerChannel bitsPerChannel;

	void *textureMemory;

	inline XMFLOAT2 SampleGrayscale8(UINT u, UINT v, TextureResolution sampleResolution);
	inline XMFLOAT2 SampleGrayscale16(UINT u, UINT v, TextureResolution sampleResolution);
	inline XMFLOAT2 SampleGrayscale32(UINT u, UINT v, TextureResolution sampleResolution);
	inline XMFLOAT4 SampleColor8(UINT u, UINT v, TextureResolution sampleResolution);
	inline XMFLOAT4 SampleColor16(UINT u, UINT v, TextureResolution sampleResolution);
	inline XMFLOAT4 SampleColor32(UINT u, UINT v, TextureResolution sampleResolution);

	public:

	TextureMemory(TextureType textureType_, TextureResolution textureResolution_, BitsPerChannel bitsPerChannel_);
	~TextureMemory();

	TextureType GetTextureType();
	TextureResolution GetResolution();
	BitsPerChannel GetFormat();
	void *GetMemoryPtr();

	XMFLOAT2 SampleGrayscale(UINT u, UINT v, TextureResolution sampleResolution);
	XMFLOAT4 SampleColor(UINT u, UINT v, TextureResolution sampleResolution);

	void SetValue(UINT u, UINT v, XMFLOAT2 value);
	void SetValue(UINT u, UINT v, XMFLOAT4 value);
};

