#include "stdafx.h"
#include "Environment.h"


Environment::Environment()
{
	directionalLight.color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	directionalLight.intensity = 3.14f;
	directionalLight.direction = XMFLOAT2(0.0f, 0.0f);
	directionalLight.isEnabled = 1;

	for(int i = 0; i < 4; i++)
	{
		sphereLights[i].color = XMFLOAT3(1.0f, 1.0f, 1.0f);
		sphereLights[i].intensity = 3.14f;
		sphereLights[i].position = XMFLOAT3(0.0f, 2.0f, 0.0f);
		sphereLights[i].lightRadius = 10.0f;
		sphereLights[i].sourceRadius = 0.1f;
		sphereLights[i].isEnabled = 0;
	}

	vertexShader = DirectXObjectPool::GetVertexShader("Environment");
	pixelShader = DirectXObjectPool::GetPixelShader("Environment");
	rasterizerState = DirectXObjectPool::GetRasterizerState("CullFront");
	linearWrapSamplerState = DirectXObjectPool::GetSamplerState("LinearWrap");
	anistotropicWrapSamplerState = DirectXObjectPool::GetSamplerState("AnisotropicWrap");
	linearClampSamplerState = DirectXObjectPool::GetSamplerState("LinearClamp");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Environment");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Environment");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Environment");

	environmentMappingIsEnabled = false;
}


Environment::~Environment()
{

}


DirectionalLight Environment::GetDirectionalLight()
{
	return directionalLight;
}


SphereLight Environment::GetSphereLight(int sphereLightNum)
{
	if(sphereLightNum < 0 || sphereLightNum >= 4)
	{
		return SphereLight();
	}

	return sphereLights[sphereLightNum];
}


int Environment::GetEnvironmentMapIndex()
{
	return environmentMapIndex;
}


void Environment::SetDirectionalLight(DirectionalLight dirLight)
{
	directionalLight = dirLight;
}


void Environment::SetSphereLight(int sphereLightNum, SphereLight punctualLight)
{
	if(sphereLightNum >= 0 && sphereLightNum < 4)
	{
		sphereLights[sphereLightNum] = punctualLight;
	}
}


void Environment::SetEnvironmentMap(LPCWSTR fileName, int environmentMapIndex_)
{
	environmentMapIndex = environmentMapIndex_;

	environmentMappingIsEnabled = false;

	environmentMap = make_shared<DirectXTexture>(fileName);

	shared_ptr<RadianceMapRenderer> radianceMapRenderer = make_shared<RadianceMapRenderer>(environmentMap, 2048);
	radianceMap = make_shared<DirectXTexture>(radianceMapRenderer);

	shared_ptr<BRDFLUTRenderer> brdfLUTRenderer = make_shared<BRDFLUTRenderer>(2048);
	brdfLUT = make_shared<DirectXTexture>(brdfLUTRenderer);

	shared_ptr<IrradianceSmoothMapRenderer> irradianceSmoothMapRenderer = make_shared<IrradianceSmoothMapRenderer>(environmentMap, 512);
	irradianceSmoothMap = make_shared<DirectXTexture>(irradianceSmoothMapRenderer);

	shared_ptr<IrradianceRoughMapRenderer> irradianceRoughMapRenderer = make_shared<IrradianceRoughMapRenderer>(environmentMap, 512);
	irradianceRoughMap = make_shared<DirectXTexture>(irradianceRoughMapRenderer);

	shared_ptr<IrradianceMultiMapRenderer> irradianceMultiMapRenderer = make_shared<IrradianceMultiMapRenderer>(environmentMap, 512);
	irradianceMultiMap = make_shared<DirectXTexture>(irradianceMultiMapRenderer);

	environmentMappingIsEnabled = true;
}


void Environment::Set()
{
	float sinPhi = sin(directionalLight.direction.x * _Pi / 180.0f);
	float cosPhi = cos(directionalLight.direction.x * _Pi / 180.0f);
	float sinPsi = sin(directionalLight.direction.y * _Pi / 180.0f);
	float cosPsi = cos(directionalLight.direction.y * _Pi / 180.0f);

	EnvironmentCB environmentCB;
	environmentCB.sphereLightsNum = 0;
	environmentCB.radianceMapMipLevelsFactor = 9;
	environmentCB.directionalLight.intensity = XMFLOAT4(directionalLight.intensity * directionalLight.color.x, directionalLight.intensity * directionalLight.color.y, directionalLight.intensity * directionalLight.color.z, 1.0f);
	environmentCB.directionalLight.direction = XMFLOAT3(sinPhi * cosPsi, -cosPhi, sinPhi * sinPsi);
	environmentCB.directionalLight.isEnabled = directionalLight.isEnabled;
	for(int i = 0; i < 4; i++)
	{
		if(sphereLights[i].isEnabled)
		{
			environmentCB.sphereLights[environmentCB.sphereLightsNum].intensity = XMFLOAT3(sphereLights[i].intensity * sphereLights[i].color.x, sphereLights[i].intensity * sphereLights[i].color.y, sphereLights[i].intensity * sphereLights[i].color.z);;
			environmentCB.sphereLights[environmentCB.sphereLightsNum].position = sphereLights[i].position;
			environmentCB.sphereLights[environmentCB.sphereLightsNum].lightRadius = sphereLights[i].lightRadius;
			environmentCB.sphereLights[environmentCB.sphereLightsNum].sourceRadius = sphereLights[i].sourceRadius;
			environmentCB.sphereLightsNum++;
		}
	}

	constantBuffer->Update(&environmentCB);
	constantBuffer->Set(1);

	if(environmentMappingIsEnabled)
	{
		anistotropicWrapSamplerState->Set(0);
		linearClampSamplerState->Set(1);

		radianceMap->Set(0);
		brdfLUT->Set(1);

		irradianceSmoothMap->Set(2);
		irradianceRoughMap->Set(3);
		irradianceMultiMap->Set(4);
	}
}


void Environment::Render()
{
	if(environmentMappingIsEnabled)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		linearWrapSamplerState->Set(2);
		environmentMap->Set(5);

		polygonMesh->Render();
	}
}