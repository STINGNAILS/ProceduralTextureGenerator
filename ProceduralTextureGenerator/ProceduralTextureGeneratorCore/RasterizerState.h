#pragma once

#include "DirectXDevice.h"


class RasterizerState
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11RasterizerState *rasterizerState;

	public:

	RasterizerState();
	~RasterizerState();

	HRESULT Init(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);

	void Set();
};

