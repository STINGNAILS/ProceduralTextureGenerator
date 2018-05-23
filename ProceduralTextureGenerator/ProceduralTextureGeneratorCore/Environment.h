#pragma once

#include "DirectXDependencies.h"
#include "DirectXDevice.h"
#include "Camera.h"
#include "DirectXTexture.h"
#include "RadianceMapRenderer.h"
#include "BRDFLUTRenderer.h"
#include "IrradianceSmoothMapRenderer.h"
#include "IrradianceRoughMapRenderer.h"
#include "IrradianceMultiMapRenderer.h"


struct DirectionalLight
{
	XMFLOAT3 color;
	float intensity;
	XMFLOAT2 direction;
	int isEnabled;
};


struct SphereLight
{
	XMFLOAT3 color;
	float intensity;
	XMFLOAT3 position;
	float lightRadius;
	float sourceRadius;
	int isEnabled;
};


struct DirectionalLightCB
{
	XMFLOAT4 intensity;
	XMFLOAT3 direction;
	int isEnabled;
};


struct SphereLightCB
{
	XMFLOAT3 intensity;
	float lightRadius;
	XMFLOAT3 position;
	float sourceRadius;
};


struct EnvironmentCB
{
	int sphereLightsNum;
	int radianceMapMipLevelsFactor;
	int aligner1;
	int aligner2;
	DirectionalLightCB directionalLight;
	SphereLightCB sphereLights[4];
};


class Environment
{
	private:

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<SamplerState> anistotropicWrapSamplerState;
	shared_ptr<SamplerState> linearClampSamplerState;
	shared_ptr<SamplerState> linearWrapSamplerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	bool environmentMappingIsEnabled;

	int environmentMapIndex;

	shared_ptr<DirectXTexture> environmentMap;

	shared_ptr<DirectXTexture> radianceMap;
	shared_ptr<DirectXTexture> brdfLUT;

	shared_ptr<DirectXTexture> irradianceSmoothMap;
	shared_ptr<DirectXTexture> irradianceRoughMap;
	shared_ptr<DirectXTexture> irradianceMultiMap;

	DirectionalLight directionalLight;
	SphereLight sphereLights[4];

	public:

	Environment();
	~Environment();

	DirectionalLight GetDirectionalLight();
	SphereLight GetSphereLight(int index);
	int GetEnvironmentMapIndex();

	void SetDirectionalLight(DirectionalLight dirLight);
	void SetSphereLight(int index, SphereLight sphereLight);
	void SetEnvironmentMap(LPCWSTR fileName, int environmentMapIndex_);

	void Set();

	void Render();
};

