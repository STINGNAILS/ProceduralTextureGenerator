#pragma once

#include "DirectXDevice.h"
#include "ShaderCompiler.h"


class PixelShader
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11PixelShader *pixelShader;

	public:

	PixelShader(LPCWSTR name);
	~PixelShader();

	void Set();
};

