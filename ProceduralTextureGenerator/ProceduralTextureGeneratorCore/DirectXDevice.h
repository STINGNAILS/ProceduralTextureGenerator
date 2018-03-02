#pragma once

#include "DirectXDependencies.h"

class DirectXDevice
{
	private:

	static bool isInitialized;

	static ID3D11Device *device;
	static ID3D11DeviceContext *painter;

	DirectXDevice();
	~DirectXDevice();

	public:

	static HRESULT Init();
	static void Release();

	static bool IsInitialized();

	static ID3D11Device *GetDevice();
	static ID3D11DeviceContext *GetPainter();
};

