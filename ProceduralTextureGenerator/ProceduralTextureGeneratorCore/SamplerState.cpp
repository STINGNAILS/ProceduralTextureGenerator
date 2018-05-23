#include "stdafx.h"
#include "SamplerState.h"


SamplerState::SamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy)
{
	samplerState = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	samplerDesc.MaxAnisotropy = maxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = 20.0f;

	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if(FAILED(hr))
	{
		throw "Couldn't create sampler state";
	}
}


SamplerState::~SamplerState()
{
	if(samplerState) samplerState->Release();
}


void SamplerState::Set(int slot)
{
	painter->VSSetSamplers(slot, 1, &samplerState);
	painter->PSSetSamplers(slot, 1, &samplerState);
}
