#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXDevice.h"
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

	ID3D11ShaderResourceView *baseColorMapSRV;
	ID3D11ShaderResourceView *metallicMapSRV;
	ID3D11ShaderResourceView *roughnessMapSRV;
	ID3D11ShaderResourceView *normalMapSRV;
	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	Cube();
	~Cube();

	HRESULT Init(shared_ptr<DirectXDevice> device_);

	void SetBaseColorMap(vector<float> &baseColorMap);
	void SetMetallicMap(vector<float> &metallicMap);
	void SetRoughnessMap(vector<float> &roughnessMap);
	void SetNormalMap(vector<float> &normalMap);

	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void ReleaseTextures();
	void Release();
};

