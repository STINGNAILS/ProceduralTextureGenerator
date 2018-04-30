#pragma once

#include "DirectXDevice.h"
#include "ShaderCompiler.h"


class VertexShader
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11InputLayout *inputLayout;
	ID3D11VertexShader *vertexShader;

	public:

	VertexShader(LPCWSTR name, D3D11_INPUT_ELEMENT_DESC *layout, UINT arraysize);
	~VertexShader();

	void Set();
};

