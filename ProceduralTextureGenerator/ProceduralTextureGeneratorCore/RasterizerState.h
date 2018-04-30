#pragma once

#include "DirectXDevice.h"


class RasterizerState
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11RasterizerState *rasterizerState;

	public:

	RasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
	~RasterizerState();

	void Set();
};

