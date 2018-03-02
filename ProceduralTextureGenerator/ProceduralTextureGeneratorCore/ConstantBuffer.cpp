#include "stdafx.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer()
{
	constantBuffer = nullptr;

	isInitialized = false;
}


ConstantBuffer::~ConstantBuffer()
{
	if(constantBuffer) constantBuffer->Release();
}


HRESULT ConstantBuffer::Init(UINT byteWidth)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(constantBuffer) constantBuffer->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bufferDesc, 0, &constantBuffer);
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


void ConstantBuffer::Update(void *cbData)
{
	if(isInitialized)
	{
		painter->UpdateSubresource(constantBuffer, 0, 0, cbData, 0, 0);
	}
}


void ConstantBuffer::Set(int slot)
{
	if(isInitialized)
	{
		painter->VSSetConstantBuffers(slot, 1, &constantBuffer);
		painter->PSSetConstantBuffers(slot, 1, &constantBuffer);
	}
}
