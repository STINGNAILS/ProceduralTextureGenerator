#pragma once

#include <memory>
#include "DirectXDevice.h"
#include "DirectXRenderer.h"
#include "ShaderCompiler.h"


class BRDFLUTRenderer : public DirectXRenderer
{
	private:

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11RasterizerState *rasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	int size;

	public:

	BRDFLUTRenderer(shared_ptr<DirectXDevice> device_);
	~BRDFLUTRenderer();

	HRESULT Init(int size_);

	HRESULT Render(ID3D11Texture2D **brdfLUT);

	void Release();
};