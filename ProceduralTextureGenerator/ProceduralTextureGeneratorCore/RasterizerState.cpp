#include "stdafx.h"
#include "RasterizerState.h"


RasterizerState::RasterizerState()
{
	rasterizerState = nullptr;

	isInitialized = false;
}


RasterizerState::~RasterizerState()
{
	if(rasterizerState) rasterizerState->Release();
}


HRESULT RasterizerState::Init(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(rasterizerState) rasterizerState->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = fillMode;
	rasterizerDesc.CullMode = cullMode;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

	return hr;
}


void RasterizerState::Set()
{
	if(isInitialized)
	{
		painter->RSSetState(rasterizerState);
	}
}
