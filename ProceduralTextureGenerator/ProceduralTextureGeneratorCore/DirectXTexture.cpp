#include "stdafx.h"
#include "DirectXTexture.h"


DirectXTexture::DirectXTexture(TextureMemoryPtr textureMemoryPtr)
{
	textureSRV = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
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
					InitGrayscale8(textureMemoryPtr);
					break;
				}
				case BPC16:
				{
					InitGrayscale16(textureMemoryPtr);
					break;
				}
				case BPC32:
				{
					InitGrayscale32(textureMemoryPtr);
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
					InitColor8(textureMemoryPtr);
					break;
				}
				case BPC16:
				{
					InitColor16(textureMemoryPtr);
					break;
				}
				case BPC32:
				{
					InitColor32(textureMemoryPtr);
					break;
				}
			}

			break;
		}
	}
}


DirectXTexture::DirectXTexture(LPCWSTR fileName)
{
	textureSRV = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	textureType = COLOR;

	ID3D11Resource *resource;
	hr = CreateDDSTextureFromFile(device, fileName, &resource, &textureSRV);
	if(resource) resource->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create texture from file";
	}
}


DirectXTexture::DirectXTexture(shared_ptr<DirectXRenderer> renderer)
{
	textureSRV = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	textureType = COLOR;

	ID3D11Texture2D *texture = nullptr;
	hr = renderer->Render(&texture);
	if(FAILED(hr))
	{
		if(texture) texture->Release();
		throw "Couldn't create texture by rendering";
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
		throw "Couldn't create shader resource view";
	}
}


DirectXTexture::~DirectXTexture()
{
	if(textureSRV) textureSRV->Release();
}


int DirectXTexture::GetMipLevelsNum(int textureSize)
{
	int mipLevelsNum = 1;
	int mipLevelSize = textureSize;

	while(mipLevelSize > 1)
	{
		mipLevelSize = max(mipLevelSize / 2, 1);
		mipLevelsNum++;
	}

	return mipLevelsNum;
}


void DirectXTexture::InitGrayscale8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMUBYTEN2);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(GRAYSCALE, mipResolution, BPC8);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleGrayscale(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMUBYTEN2);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
}


void DirectXTexture::InitGrayscale16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMUSHORTN2);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(GRAYSCALE, mipResolution, BPC16);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleGrayscale(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMUSHORTN2);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
}


void DirectXTexture::InitGrayscale32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMFLOAT2);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(GRAYSCALE, mipResolution, BPC32);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleGrayscale(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMFLOAT2);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
}


void DirectXTexture::InitColor8(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMUBYTEN4);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(COLOR, mipResolution, BPC8);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleColor(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMUBYTEN4);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
}


void DirectXTexture::InitColor16(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMUSHORTN4);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(COLOR, mipResolution, BPC16);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleColor(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMUSHORTN4);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
}


void DirectXTexture::InitColor32(TextureMemoryPtr textureMemoryPtr)
{
	HRESULT hr = S_OK;

	TextureResolution resolution = textureMemoryPtr->GetResolution();

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.MipLevels = GetMipLevelsNum(resolution);
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	vector<TextureMemoryPtr> memory(textureDesc.MipLevels);
	vector<D3D11_SUBRESOURCE_DATA> textureData(textureDesc.MipLevels);

	memory[0] = textureMemoryPtr;
	ZeroMemory(&textureData[0], sizeof(D3D11_SUBRESOURCE_DATA));
	textureData[0].pSysMem = memory[0]->GetMemoryPtr();
	textureData[0].SysMemPitch = resolution * sizeof(XMFLOAT4);
	textureData[0].SysMemSlicePitch = 0;

	for(int k = 1; k < memory.size(); k++)
	{
		TextureResolution mipResolution = (TextureResolution) (memory[k - 1]->GetResolution() / 2);

		memory[k] = make_shared<TextureMemory>(COLOR, mipResolution, BPC32);

		for(int i = 0; i < mipResolution; i++)
		{
			for(int j = 0; j < mipResolution; j++)
			{
				memory[k]->SetValue(i, j, memory[k - 1]->SampleColor(i, j, mipResolution));
			}
		}

		ZeroMemory(&textureData[k], sizeof(D3D11_SUBRESOURCE_DATA));
		textureData[k].pSysMem = memory[k]->GetMemoryPtr();
		textureData[k].SysMemPitch = mipResolution * sizeof(XMFLOAT4);
		textureData[k].SysMemSlicePitch = 0;
	}

	ID3D11Texture2D *texture = 0;

	hr = device->CreateTexture2D(&textureDesc, &textureData[0], &texture);
	if(FAILED(hr))
	{
		throw "Couldn't create texture";
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	texture->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create shader resource view";
	}
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