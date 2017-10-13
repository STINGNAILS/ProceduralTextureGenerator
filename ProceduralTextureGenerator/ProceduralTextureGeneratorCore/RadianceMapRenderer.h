#pragma once

#include <memory>
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

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *rasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	int size;
	shared_ptr<DirectXTexture> environmentMap;

	int MipLevelsNum(int size);

	public:

	RadianceMapRenderer(shared_ptr<DirectXDevice> device_);
	~RadianceMapRenderer();

	HRESULT Init(shared_ptr<DirectXTexture> environmentMap_, int size_);

	HRESULT Render(ID3D11Texture2D **radianceMap);

	void Release();
};