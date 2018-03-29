#pragma once

#include "DirectXDevice.h"
#include "TextureMemory.h"
#include "DirectXRenderer.h"
#include "DDSTextureLoader.h"


#define DirectXTexturePtr shared_ptr<DirectXTexture>


class DirectXTexture
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;
	
	ID3D11ShaderResourceView *textureSRV;

	TextureType textureType;

	HRESULT InitGrayscale8(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitGrayscale16(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitGrayscale32(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor8(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor16(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitColor32(TextureMemoryPtr textureMemoryPtr);

	public:

	DirectXTexture();
	~DirectXTexture();

	HRESULT InitFromMemory(TextureMemoryPtr textureMemoryPtr);
	HRESULT InitFromFile(LPCWSTR fileName);
	HRESULT InitFromRenderer(shared_ptr<DirectXRenderer> renderer);

	TextureType GetTextureType();

	void Set(int slot);
};

