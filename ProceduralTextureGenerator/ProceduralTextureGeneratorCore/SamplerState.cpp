#include "stdafx.h"
#include "SamplerState.h"


SamplerState::SamplerState()
{
	samplerState = nullptr;

	isInitialized = false;
}


SamplerState::~SamplerState()
{

}


HRESULT SamplerState::Init(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy)
{
	HRESULT hr = S_OK;

	isInitialized = false;

	if(samplerState) samplerState->Release();

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

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
		return hr;
	}

	isInitialized = true;

	return hr;
}


void SamplerState::Set(int slot)
{
	if(isInitialized)
	{
		painter->VSSetSamplers(slot, 1, &samplerState);
		painter->PSSetSamplers(slot, 1, &samplerState);
	}
}
