#pragma once

#include "DirectXDevice.h"
#include "DirectXRenderer.h"
#include "DirectXTexture.h"
#include "ShaderCompiler.h"


struct RadianceMapCB
{
	float roughness;
	int faceIndex;
	int aligner1;
	int aligner2;
};


class RadianceMapRenderer : public DirectXRenderer
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

	int MipLevelsNum(int size);

	public:

	RadianceMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_);
	~RadianceMapRenderer();

	HRESULT Render(ID3D11Texture2D **radianceMap);
};