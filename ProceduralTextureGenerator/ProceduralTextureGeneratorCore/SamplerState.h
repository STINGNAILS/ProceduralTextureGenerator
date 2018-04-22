#pragma once

#include "DirectXDevice.h"


class SamplerState
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11SamplerState *samplerState;

	public:

	SamplerState();
	~SamplerState();

	HRESULT Init(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy);

	void Set(int slot);
};

