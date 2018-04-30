#include "stdafx.h"
#include "VertexShader.h"


VertexShader::VertexShader(LPCWSTR name, D3D11_INPUT_ELEMENT_DESC *layout, UINT arraysize)
{
	inputLayout = nullptr;
	vertexShader = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();
	
	HRESULT hr = S_OK;

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(name, "VS", "vs_5_0", &shaderBlob);
	if(FAILED(hr))
	{
		throw "Couldn't compile vertex shader";
	}

	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
	if(FAILED(hr))
	{
		shaderBlob->Release();
		throw "Couldn't create vertex shader";
	}

	hr = device->CreateInputLayout(layout, arraysize, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
	shaderBlob->Release();
	if(FAILED(hr))
	{
		throw "Couldn't create input layout";
	}
}


VertexShader::~VertexShader()
{
	if(inputLayout) inputLayout->Release();
	if(vertexShader) vertexShader->Release();
}


void VertexShader::Set()
{
	painter->IASetInputLayout(inputLayout);
	painter->VSSetShader(vertexShader, 0, 0);
}
