#pragma once

#include "DirectXDevice.h"
#include "TextureMemory.h"
#include "DirectXRenderer.h"
#include "DDSTextureLoader.h"


#define DirectXTexturePtr shared_ptr<DirectXTexture>


enum BitsPerChannel
{
	BPC8,
	BPC16,
	BPC32
};


class DirectXTexture
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11ShaderResourceView *textureSRV;

	HRESULT InitGrayscale8(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitGrayscale16(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitGrayscale32(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor8(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor16(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor32(TextureMemoryPtr textureMemoryPtr);

	public:

	DirectXTexture();
	~DirectXTexture();

	HRESULT InitFromMemory(TextureMemoryPtr textureMemoryPtr, BitsPerChannel bpc);
	HRESULT InitFromFile(LPCWSTR fileName);
	HRESULT InitFromRenderer(shared_ptr<DirectXRenderer> renderer);

	void Set(int slot);
};

