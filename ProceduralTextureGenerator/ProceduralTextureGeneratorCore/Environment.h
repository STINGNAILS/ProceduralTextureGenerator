#pragma once

#include <memory>
#include "DirectXDependencies.h"
#include "DirectXDevice.h"
#include "Camera.h"
#include "RadianceMapRenderer.h"
#include "BRDFLUTRenderer.h"
#include "DDSTextureLoader.h"


using namespace std;
using namespace DirectX;


struct DirectionalLight
{
	XMFLOAT4 intensity;
	XMFLOAT4 direction;
};


struct PunctualLight
{
	XMFLOAT4 intensity;
	XMFLOAT4 position;
};


struct EnvironmentCB
{
	int directionalLightsNum;
	int radianceMapMipLevelsFactor;
	int aligner1;
	int aligner2;
	DirectionalLight directionalLights[4];
};


class Environment
{
	private:

	int lastDirectionalLightNum;
	int lastPunctualLightNum;

	map<int, DirectionalLight> directionalLights;
	map<int, PunctualLight> punctualLights;

	bool fxIsInitialized;
	bool geometryIsInitialized;
	bool cubeMapIsInitialized;

	shared_ptr<DirectXDevice> device;

	ID3D11ShaderResourceView *cubeMapSRV;
	ID3D11ShaderResourceView *radianceMapSRV;
	ID3D11ShaderResourceView *brdfLUTSRV;
	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *environmentConstantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	Environment();
	~Environment();

	int AddDirectionalLight(DirectionalLight dirLight);
	int AddPunctualLight(PunctualLight punctualLight);
	DirectionalLight GetDirectionalLight(int directionalLightNum);
	PunctualLight GetPunctualLight(int punctualLightNum);
	void ModifyDirectionalLight(int directionalLightNum, DirectionalLight dirLight);
	void ModifyPunctualLight(int punctualLightNum, PunctualLight punctualLight);
	void RemoveDirectionalLight(int directionalLightNum);
	void RemovePunctualLight(int punctualLightNum);

	HRESULT Init(shared_ptr<DirectXDevice> device_, LPCWSTR fileName);

	void Prepare();

	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void ReleaseCubeMap();
	void Release();
};

