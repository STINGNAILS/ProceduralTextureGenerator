#pragma once

#include "DirectXDependencies.h"

class DirectXDevice
{
	private:

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	public:

	DirectXDevice();
	~DirectXDevice();

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetPainter();

	HRESULT Init();

	void Release();
};

