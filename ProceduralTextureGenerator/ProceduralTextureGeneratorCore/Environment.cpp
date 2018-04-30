#include "stdafx.h"
#include "Environment.h"


Environment::Environment()
{
	lastDirectionalLightNum = 0;
	lastPunctualLightNum = 0;

	environmentMappingIsEnabled = false;

	constantBuffer = DirectXObjectPool::GetConstantBuffer("Environment");
}


Environment::Environment(LPCWSTR fileName)
{
	lastDirectionalLightNum = 0;
	lastPunctualLightNum = 0;

	vertexShader = DirectXObjectPool::GetVertexShader("Environment");
	pixelShader = DirectXObjectPool::GetPixelShader("Environment");
	rasterizerState = DirectXObjectPool::GetRasterizerState("CullFront");
	basicSamplerState = DirectXObjectPool::GetSamplerState("Basic");
	anistotropicSamplerState = DirectXObjectPool::GetSamplerState("Anisotropic");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Environment");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Environment");

	environmentMap = make_shared<DirectXTexture>(fileName);

	shared_ptr<RadianceMapRenderer> radianceMapRenderer = make_shared<RadianceMapRenderer>(environmentMap, 2048);
	radianceMap = make_shared<DirectXTexture>(radianceMapRenderer);

	shared_ptr<BRDFLUTRenderer> brdfLUTRenderer = make_shared<BRDFLUTRenderer>(2048);
	brdfLUT = make_shared<DirectXTexture>(brdfLUTRenderer);

	environmentMappingIsEnabled = true;
}


Environment::~Environment()
{

}


int Environment::AddDirectionalLight(DirectionalLight dirLight)
{
	lastDirectionalLightNum++;
	directionalLights[lastDirectionalLightNum] = dirLight;
	return lastDirectionalLightNum;
}


int Environment::AddPunctualLight(PunctualLight punctualLight)
{
	lastPunctualLightNum++;
	punctualLights[lastPunctualLightNum] = punctualLight;
	return lastPunctualLightNum;
}


DirectionalLight Environment::GetDirectionalLight(int directionalLightNum)
{
	return directionalLights[directionalLightNum];
}


PunctualLight Environment::GetPunctualLight(int punctualLightNum)
{
	return punctualLights[punctualLightNum];
}


void Environment::ModifyDirectionalLight(int directionalLightNum, DirectionalLight dirLight)
{
	directionalLights[directionalLightNum] = dirLight;
}


void Environment::ModifyPunctualLight(int punctualLightNum, PunctualLight punctualLight)
{
	punctualLights[punctualLightNum] = punctualLight;
}

void Environment::RemoveDirectionalLight(int directionalLightNum)
{
	directionalLights.erase(directionalLightNum);
}


void Environment::RemovePunctualLight(int punctualLightNum)
{
	punctualLights.erase(punctualLightNum);
}


void Environment::Set()
{
	EnvironmentCB environmentCB;
	environmentCB.directionalLightsNum = min(directionalLights.size(), 4);
	environmentCB.radianceMapMipLevelsFactor = 9;

	int i = 0;
	for(auto it = directionalLights.begin(); i < 4, it != directionalLights.end(); it++)
	{
		environmentCB.directionalLights[i] = it->second;
		i++;
	}

	constantBuffer->Update(&environmentCB);
	constantBuffer->Set(1);

	if(environmentMappingIsEnabled)
	{
		anistotropicSamplerState->Set(0);
		radianceMap->Set(0);
		brdfLUT->Set(1);
	}
}


void Environment::Render()
{
	if(environmentMappingIsEnabled)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		basicSamplerState->Set(1);
		environmentMap->Set(2);

		polygonMesh->Render();
	}
}