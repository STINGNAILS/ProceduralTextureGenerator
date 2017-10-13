#pragma once

#include <memory>
#include "DirectXDevice.h"
#include "TextureMemory.h"
#include "DirectXRenderer.h"
#include "DDSTextureLoader.h"


enum BitsPerChannel
{
	BPC8,
	BPC16,
	BPC32
};


class DirectXTexture
{
	private:

	shared_ptr<DirectXDevice> device;

	ID3D11ShaderResourceView *textureSRV;

	HRESULT InitGrayscale8(shared_ptr<TextureMemory> textureMemoryPtr);
	HRESULT InitGrayscale16(shared_ptr<TextureMemory> textureMemoryPtr);
	HRESULT InitGrayscale32(shared_ptr<TextureMemory> textureMemoryPtr);
	HRESULT InitColor8(shared_ptr<TextureMemory> textureMemoryPtr);
	HRESULT InitColor16(shared_ptr<TextureMemory> textureMemoryPtr);
	HRESULT InitColor32(shared_ptr<TextureMemory> textureMemoryPtr);

	public:

	DirectXTexture(shared_ptr<DirectXDevice> device_);
	~DirectXTexture();

	HRESULT InitFromMemory(shared_ptr<TextureMemory> textureMemoryPtr, BitsPerChannel bpc);
	HRESULT InitFromFile(LPCWSTR fileName);
	HRESULT InitFromRenderer(shared_ptr<DirectXRenderer> renderer);

	void Set(int slot);

	void Release();
};

