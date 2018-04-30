#pragma once

#include "DirectXDevice.h"
#include "DirectXRenderer.h"
#include "ShaderCompiler.h"


class BRDFLUTRenderer : public DirectXRenderer
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;

	int size;

	public:

	BRDFLUTRenderer(int size_);
	~BRDFLUTRenderer();

	HRESULT Render(ID3D11Texture2D **brdfLUT);
};