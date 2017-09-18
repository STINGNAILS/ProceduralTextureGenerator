#pragma once

#include <memory>
#include "DirectXDevice.h"
#include "ShaderCompiler.h"


using namespace std;
using namespace DirectX;


struct RadianceMapCB
{
	float roughness;
	int faceIndex;
	int aligner1;
	int aligner2;
};


class RadianceMapRenderer
{
	private:

	shared_ptr<DirectXDevice> device;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *rasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	int MipLevelsNum(int size);

	public:

	RadianceMapRenderer();
	~RadianceMapRenderer();

	HRESULT Init(shared_ptr<DirectXDevice> device_);

	HRESULT Render(int size, ID3D11ShaderResourceView *environmentMapSRV, ID3D11Texture2D **radianceMap);

	void Release();
};