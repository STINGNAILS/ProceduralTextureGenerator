#include "stdafx.h"
#include "DirectXTexture.h"


DirectXTexture::DirectXTexture()
{
	textureSRV = nullptr;

	isInitialized = false;
}


DirectXTexture::~DirectXTexture()
{
	if(textureSRV) textureSRV->Release();
}


HRESULT DirectXTexture::InitGrayscale8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	
	XMUBYTEN2 *memory = (XMUBYTEN2*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMUBYTEN2);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitGrayscale16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUSHORTN2 *memory = (XMUSHORTN2*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMUSHORTN2);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitGrayscale32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMFLOAT2 *memory = (XMFLOAT2*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMFLOAT2);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitColor8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUBYTEN4 *memory = (XMUBYTEN4*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMUBYTEN4);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitColor16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMUSHORTN4 *memory = (XMUSHORTN4*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMUSHORTN4);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitColor32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	XMFLOAT4 *memory = (XMFLOAT4*) textureMemoryPtr->GetMemoryPtr();

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = &memory[0];
	textureData.SysMemPitch = resolution * sizeof(XMFLOAT4);
	textureData.SysMemSlicePitch = 0;
	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData, &texture);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

	texture->Release();

	return hr;
}


HRESULT DirectXTexture::InitFromMemory(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(textureSRV) textureSRV->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	textureType = textureMemoryPtr->GetTextureType();
	BitsPerChannel bpc = textureMemoryPtr->GetFormat();

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
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


HRESULT DirectXTexture::InitFromFile(LPCWSTR fileName)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(textureSRV) textureSRV->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	textureType = COLOR;

	ID3D11Resource *resource;
	hr = CreateDDSTextureFromFile(device, fileName, &resource, &textureSRV);
	if(resource) resource->Release();
	if(FAILED(hr))
	{
		return hr;
	}
	
	isInitialized = true;
	
	return hr;
}


HRESULT DirectXTexture::InitFromRenderer(shared_ptr<DirectXRenderer> renderer)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(textureSRV) textureSRV->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	textureType = COLOR;

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
	
	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


TextureType DirectXTexture::GetTextureType()
{
	return textureType;
}


void DirectXTexture::Set(int slot)
{
	painter->VSSetShaderResources(slot, 1, &textureSRV);
	painter->PSSetShaderResources(slot, 1, &textureSRV);
}