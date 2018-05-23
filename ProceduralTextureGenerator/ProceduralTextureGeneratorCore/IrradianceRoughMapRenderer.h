#pragma once

#include "DirectXDevice.h"
#include "DirectXRenderer.h"
#include "DirectXTexture.h"


struct IrradianceRoughMapCB
{
	int faceIndex;
	int aligner1;
	int aligner2;
	int aligner3;
};


class IrradianceRoughMapRenderer : public DirectXRenderer
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<SamplerState> samplerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	int size;
	shared_ptr<DirectXTexture> environmentMap;

	public:

	IrradianceRoughMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_);
	~IrradianceRoughMapRenderer();

	HRESULT Render(ID3D11Texture2D **irradianceRoughMap);
};

