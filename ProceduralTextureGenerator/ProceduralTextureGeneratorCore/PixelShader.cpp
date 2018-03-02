#include "stdafx.h"
#include "PixelShader.h"


PixelShader::PixelShader()
{
	pixelShader = nullptr;

	isInitialized = false;
}


PixelShader::~PixelShader()
{
	if(pixelShader) pixelShader->Release();
}


HRESULT PixelShader::Init(LPCWSTR name)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(pixelShader) pixelShader->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(name, "PS", "ps_5_0", &shaderBlob);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
	shaderBlob->Release();
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


void PixelShader::Set()
{
	if(isInitialized)
	{
		painter->PSSetShader(pixelShader, 0, 0);
	}
}
