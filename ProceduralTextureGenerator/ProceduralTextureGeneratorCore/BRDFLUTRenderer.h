#pragma once

#include <memory>
#include "DirectXDevice.h"
#include "ShaderCompiler.h"


using namespace std;
using namespace DirectX;


class BRDFLUTRenderer
{
	private:

	shared_ptr<DirectXDevice> device;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11RasterizerState *rasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	BRDFLUTRenderer();
	~BRDFLUTRenderer();

	HRESULT Init(shared_ptr<DirectXDevice> device_);

	HRESULT Render(int size, ID3D11Texture2D **brdfLUT);

	void Release();
};