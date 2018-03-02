#pragma once

#include "DirectXDevice.h"
#include "ShaderCompiler.h"


class VertexShader
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11InputLayout *inputLayout;
	ID3D11VertexShader *vertexShader;

	public:

	VertexShader();
	~VertexShader();

	HRESULT Init(LPCWSTR name, D3D11_INPUT_ELEMENT_DESC *layout, UINT arraysize);

	void Set();
};

