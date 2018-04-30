#include "stdafx.h"
#include "RasterizerState.h"


RasterizerState::RasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	rasterizerState = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = fillMode;
	rasterizerDesc.CullMode = cullMode;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr))
	{
		throw "Couldn't create rasterizer state";
	}
}


RasterizerState::~RasterizerState()
{
	if(rasterizerState) rasterizerState->Release();
}


void RasterizerState::Set()
{
	painter->RSSetState(rasterizerState);
}
