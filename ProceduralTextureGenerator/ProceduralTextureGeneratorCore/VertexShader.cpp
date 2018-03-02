#include "stdafx.h"
#include "VertexShader.h"


VertexShader::VertexShader()
{
	inputLayout = nullptr;
	vertexShader = nullptr;

	isInitialized = false;
}


VertexShader::~VertexShader()
{
	if(inputLayout) inputLayout->Release();
	if(vertexShader) vertexShader->Release();
}


HRESULT VertexShader::Init(LPCWSTR name, D3D11_INPUT_ELEMENT_DESC *layout, UINT arraysize)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(vertexShader) vertexShader->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(name, "VS", "vs_5_0", &shaderBlob);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
	if(FAILED(hr))
	{
		shaderBlob->Release();
		return hr;
	}

	hr = device->CreateInputLayout(layout, arraysize, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
	shaderBlob->Release();
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


void VertexShader::Set()
{
	if(isInitialized)
	{
		painter->IASetInputLayout(inputLayout);
		painter->VSSetShader(vertexShader, 0, 0);
	}
}
