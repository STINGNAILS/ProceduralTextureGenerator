#include "stdafx.h"
#include "DirectXTexture.h"


DirectXTexture::DirectXTexture(shared_ptr<DirectXDevice> device_)
{
	device = device_;
}


DirectXTexture::~DirectXTexture()
{
	Release();
}


HRESULT DirectXTexture::InitGrayscale8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUBYTEN4 *memory = new XMUBYTEN4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			float tempGRAYSCALE = textureMemory(i, j, 0);
			XMFLOAT4 tempFLOAT4 = XMFLOAT4(tempGRAYSCALE, tempGRAYSCALE, tempGRAYSCALE, textureMemory(i, j, 1));
			XMVECTOR tempVECTOR = XMLoadFloat4(&tempFLOAT4);
			XMStoreUByteN4(&memory[i * size + j], tempVECTOR);
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(uint8_t) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitGrayscale16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUSHORTN4 *memory = new XMUSHORTN4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			float tempGRAYSCALE = textureMemory(i, j, 0);
			XMFLOAT4 tempFLOAT4 = XMFLOAT4(tempGRAYSCALE, tempGRAYSCALE, tempGRAYSCALE, textureMemory(i, j, 1));
			XMVECTOR tempVECTOR = XMLoadFloat4(&tempFLOAT4);
			XMStoreUShortN4(&memory[i * size + j], tempVECTOR);
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(uint16_t) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitGrayscale32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMFLOAT4 *memory = new XMFLOAT4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			float tempGRAYSCALE = textureMemory(i, j, 0);
			memory[i * size + j] = XMFLOAT4(tempGRAYSCALE, tempGRAYSCALE, tempGRAYSCALE, textureMemory(i, j, 1));
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(float) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitColor8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUBYTEN4 *memory = new XMUBYTEN4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			XMFLOAT4 tempFLOAT4 = XMFLOAT4(textureMemory(i, j, 0), textureMemory(i, j, 1), textureMemory(i, j, 2), textureMemory(i, j, 3));
			XMVECTOR tempVECTOR = XMLoadFloat4(&tempFLOAT4);
			XMStoreUByteN4(&memory[i * size + j], tempVECTOR);
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(uint8_t) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitColor16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUSHORTN4 *memory = new XMUSHORTN4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			XMFLOAT4 tempFLOAT4 = XMFLOAT4(textureMemory(i, j, 0), textureMemory(i, j, 1), textureMemory(i, j, 2), textureMemory(i, j, 3));
			XMVECTOR tempVECTOR = XMLoadFloat4(&tempFLOAT4);
			XMStoreUShortN4(&memory[i * size + j], tempVECTOR);
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(uint16_t) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitColor32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureMemory &textureMemory = *textureMemoryPtr.get();

	int size = textureMemory.GetSize();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMFLOAT4 *memory = new XMFLOAT4[size * size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			memory[i * size + j] = XMFLOAT4(textureMemory(i, j, 0), textureMemory(i, j, 1), textureMemory(i, j, 2), textureMemory(i, j, 3));
		}
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = size * sizeof(float) * 4;
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->GetDevice()->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		delete[] memory;
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();
	delete[] memory;

	return hr;
}


HRESULT DirectXTexture::InitFromMemory(TextureMemoryPtr textureMemoryPtr, BitsPerChannel bpc)
{
	HRESULT hr = S_OK;

	TextureType textureType = textureMemoryPtr->GetTextureType();

	switch(textureType)
	{
		case GRAYSCALE:
		{
			switch(bpc)
			{
				case BPC8:
				{
					hr = InitGrayscale8(textureMemoryPtr);
					break;
				}
				case BPC16:
				{
					hr = InitGrayscale16(textureMemoryPtr);
					break;
				}
				case BPC32:
				{
					hr = InitGrayscale32(textureMemoryPtr);
					break;
				}
			}

			break;
		}
		case COLOR:
		{
			switch(bpc)
			{
				case BPC8:
				{
					hr = InitColor8(textureMemoryPtr);
					break;
				}
				case BPC16:
				{
					hr = InitColor16(textureMemoryPtr);
					break;
				}
				case BPC32:
				{
					hr = InitColor32(textureMemoryPtr);
					break;
				}
			}

			break;
		}
	}

	return hr;
}


HRESULT DirectXTexture::InitFromFile(LPCWSTR fileName)
{
	HRESULT hr = S_OK;

	ID3D11Resource *resource;
	hr = CreateDDSTextureFromFile(device->GetDevice(), fileName, &resource, &textureSRV);

	if(resource) resource->Release();
	
	return hr;
}


HRESULT DirectXTexture::InitFromRenderer(shared_ptr<DirectXRenderer> renderer)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D *texture = nullptr;
	hr = renderer->Render(&texture);
	if(FAILED(hr))
	{
		if(texture) texture->Release();
		return hr;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = (textureDesc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) ? D3D11_SRV_DIMENSION_TEXTURECUBE : D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	
	hr = device->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


void DirectXTexture::Set(int slot)
{
	device->GetPainter()->VSSetShaderResources(slot, 1, &textureSRV);
	device->GetPainter()->PSSetShaderResources(slot, 1, &textureSRV);
}


void DirectXTexture::Release()
{
	device = nullptr;

	if(textureSRV) textureSRV->Release();
}
