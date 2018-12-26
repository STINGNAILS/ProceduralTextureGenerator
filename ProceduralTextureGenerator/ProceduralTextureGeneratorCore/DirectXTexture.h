#pragma once

#include "DirectXDevice.h"
#include "TextureMemory.h"
#include "DirectXRenderer.h"
#include "DDSTextureLoader.h"
#include "ScreenGrab.h"


#define DirectXTexturePtr shared_ptr<DirectXTexture>


class DirectXTexture
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;
	
	ID3D11ShaderResourceView *textureSRV;

	TextureType textureType;
	BitsPerChannel bpc;

	int GetMipLevelsNum(int textureSize);

	void InitGrayscale8(TextureMemoryPtr textureMemoryPtr);
	void InitGrayscale16(TextureMemoryPtr textureMemoryPtr);
	void InitGrayscale32(TextureMemoryPtr textureMemoryPtr);
	void InitColor8(TextureMemoryPtr textureMemoryPtr);
	void InitColor16(TextureMemoryPtr textureMemoryPtr);
	void InitColor32(TextureMemoryPtr textureMemoryPtr);

	public:

	DirectXTexture(TextureMemoryPtr textureMemoryPtr);
	DirectXTexture(LPCWSTR fileName);
	DirectXTexture(shared_ptr<DirectXRenderer> renderer);
	~DirectXTexture();

	TextureType GetTextureType();

	void Set(int slot);

	void SaveAsJPG(LPCWSTR fileName);
	void SaveAsPNG(LPCWSTR fileName);
	void SaveAsBMP(LPCWSTR fileName);
	void SaveAsDDS(LPCWSTR fileName);
};

