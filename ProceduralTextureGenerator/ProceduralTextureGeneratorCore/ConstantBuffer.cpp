#include "stdafx.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer(UINT byteWidth)
{
	constantBuffer = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bufferDesc, 0, &constantBuffer);
	if(FAILED(hr))
	{
		throw "Couldn't create constant buffer";
	}
}


ConstantBuffer::~ConstantBuffer()
{
	if(constantBuffer) constantBuffer->Release();
}


void ConstantBuffer::Update(void *cbData)
{
	painter->UpdateSubresource(constantBuffer, 0, 0, cbData, 0, 0);
}


void ConstantBuffer::Set(int slot)
{
	painter->VSSetConstantBuffers(slot, 1, &constantBuffer);
	painter->PSSetConstantBuffers(slot, 1, &constantBuffer);
}
