#pragma once

#include "DirectXDependencies.h"
#include "DirectXDevice.h"
#include "Camera.h"
#include "DirectXTexture.h"
#include "RadianceMapRenderer.h"
#include "BRDFLUTRenderer.h"


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

	bool isInitialized;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	bool environmentIsInitialized;

	shared_ptr<DirectXTexture> environmentMap;
	shared_ptr<DirectXTexture> radianceMap;
	shared_ptr<DirectXTexture> brdfLUT;

	int lastDirectionalLightNum;
	int lastPunctualLightNum;

	map<int, DirectionalLight> directionalLights;
	map<int, PunctualLight> punctualLights;

	public:

	Environment();
	~Environment();

	HRESULT Init();
	HRESULT InitEnvironment(LPCWSTR fileName);

	int AddDirectionalLight(DirectionalLight dirLight);
	int AddPunctualLight(PunctualLight punctualLight);
	DirectionalLight GetDirectionalLight(int directionalLightNum);
	PunctualLight GetPunctualLight(int punctualLightNum);
	void ModifyDirectionalLight(int directionalLightNum, DirectionalLight dirLight);
	void ModifyPunctualLight(int punctualLightNum, PunctualLight punctualLight);
	void RemoveDirectionalLight(int directionalLightNum);
	void RemovePunctualLight(int punctualLightNum);

	void Set();

	void Render();
};

