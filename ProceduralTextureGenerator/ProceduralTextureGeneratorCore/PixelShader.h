#pragma once

#include "DirectXDevice.h"
#include "ShaderCompiler.h"


class PixelShader
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11PixelShader *pixelShader;

	public:

	PixelShader();
	~PixelShader();

	HRESULT Init(LPCWSTR name);

	void Set();
};

