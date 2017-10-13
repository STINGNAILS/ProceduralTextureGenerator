#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXDevice.h"
#include "DirectXTexture.h"
#include "RenderableObject.h"
#include "Environment.h"
#include "ShaderCompiler.h"
#include "GeometryHelper.h"


using namespace std;


class Cube : public RenderableObject
{
	private:

	bool fxIsInitialized;
	bool geometryIsInitialized;

	shared_ptr<DirectXTexture> baseColorMap;
	shared_ptr<DirectXTexture> metallicMap;
	shared_ptr<DirectXTexture> roughnessMap;
	shared_ptr<DirectXTexture> normalMap;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	Cube(shared_ptr<DirectXDevice> device_);
	~Cube();

	HRESULT Init();

	void SetBaseColorMap(shared_ptr<DirectXTexture> baseColorMap_);
	void SetMetallicMap(shared_ptr<DirectXTexture> metallicMap_);
	void SetRoughnessMap(shared_ptr<DirectXTexture> roughnessMap_);
	void SetNormalMap(shared_ptr<DirectXTexture> normalMap_);

	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void ReleaseTextures();
	void Release();
};

