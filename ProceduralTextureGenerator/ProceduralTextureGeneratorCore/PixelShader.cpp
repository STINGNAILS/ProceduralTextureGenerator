#include "stdafx.h"
#include "PixelShader.h"


PixelShader::PixelShader(LPCWSTR name)
{
	pixelShader = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(name, "PS", "ps_5_0", &shaderBlob);
	if(FAILED(hr))
	{
		throw "Couldn't compile pixel shader";
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
	shaderBlob->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create pixel shader";
	}
}


PixelShader::~PixelShader()
{
	if(pixelShader) pixelShader->Release();
}

void PixelShader::Set()
{
	painter->PSSetShader(pixelShader, 0, 0);
}
