#include "stdafx.h"
#include "Environment.h"


struct CubeMapVertex
{
	XMFLOAT3 pos;
};


Environment::Environment()
{
	lastDirectionalLightNum = 0;
	lastPunctualLightNum = 0;

	isInitialized = false;
	environmentIsInitialized = false;
}


Environment::~Environment()
{

}


HRESULT Environment::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	constantBuffer = DirectXObjectPool::GetConstantBuffer("Environment");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(EnvironmentCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("Environment", constantBuffer);
	}

	isInitialized = true;

	return hr;
}


HRESULT Environment::InitEnvironment(LPCWSTR fileName)
{
	HRESULT hr = S_OK;

	environmentIsInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("Environment");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = vertexShader->Init(L"EnvironmentShader.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("Environment", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("Environment");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"EnvironmentShader.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("Environment", pixelShader);
	}

	rasterizerState = DirectXObjectPool::GetRasterizerState("CullFront");
	if(rasterizerState.get() == nullptr)
	{
		rasterizerState = make_shared<RasterizerState>();

		hr = rasterizerState->Init(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetRasterizerState("CullFront", rasterizerState);
	}

	polygonMesh = DirectXObjectPool::GetPolygonMesh("Environment");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<CubeMapVertex> cubeVertices(8);

		cubeVertices[0].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		cubeVertices[1].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[2].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		cubeVertices[3].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
		cubeVertices[4].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		cubeVertices[5].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
		cubeVertices[6].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		cubeVertices[7].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);

		vector<UINT> cubeIndices(36);

		cubeIndices[0] = 0;
		cubeIndices[1] = 1;
		cubeIndices[2] = 2;

		cubeIndices[3] = 1;
		cubeIndices[4] = 3;
		cubeIndices[5] = 2;

		cubeIndices[6] = 1;
		cubeIndices[7] = 5;
		cubeIndices[8] = 3;

		cubeIndices[9] = 5;
		cubeIndices[10] = 7;
		cubeIndices[11] = 3;

		cubeIndices[12] = 5;
		cubeIndices[13] = 4;
		cubeIndices[14] = 7;

		cubeIndices[15] = 4;
		cubeIndices[16] = 6;
		cubeIndices[17] = 7;

		cubeIndices[18] = 4;
		cubeIndices[19] = 0;
		cubeIndices[20] = 6;

		cubeIndices[21] = 0;
		cubeIndices[22] = 2;
		cubeIndices[23] = 6;

		cubeIndices[24] = 4;
		cubeIndices[25] = 5;
		cubeIndices[26] = 0;

		cubeIndices[27] = 5;
		cubeIndices[28] = 1;
		cubeIndices[29] = 0;

		cubeIndices[30] = 2;
		cubeIndices[31] = 3;
		cubeIndices[32] = 6;

		cubeIndices[33] = 3;
		cubeIndices[34] = 7;
		cubeIndices[35] = 6;

		hr = polygonMesh->Init((void*) &cubeVertices[0], sizeof(CubeMapVertex), cubeVertices.size(), &cubeIndices[0], cubeIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("Environment", polygonMesh);
	}

	environmentMap = make_shared<DirectXTexture>();
	hr = environmentMap->InitFromFile(fileName);
	if(FAILED(hr))
	{
		return hr;
	}

	shared_ptr<RadianceMapRenderer> radianceMapRenderer = make_shared<RadianceMapRenderer>();
	hr = radianceMapRenderer->Init(environmentMap, 2048);
	if(FAILED(hr))
	{
		return hr;
	}

	radianceMap = make_shared<DirectXTexture>();
	hr = radianceMap->InitFromRenderer(radianceMapRenderer);
	if(FAILED(hr))
	{
		return hr;
	}

	shared_ptr<BRDFLUTRenderer> brdfLUTRenderer = make_shared<BRDFLUTRenderer>();
	hr = brdfLUTRenderer->Init(2048);
	if(FAILED(hr))
	{
		return hr;
	}

	brdfLUT = make_shared<DirectXTexture>();
	hr = brdfLUT->InitFromRenderer(brdfLUTRenderer);
	if(FAILED(hr))
	{
		return hr;
	}

	environmentIsInitialized = true;

	return hr;
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
	if(isInitialized)
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

		if(environmentIsInitialized)
		{
			radianceMap->Set(0);
			brdfLUT->Set(1);
		}
	}
}


void Environment::Render()
{
	if(environmentIsInitialized)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		environmentMap->Set(0);

		polygonMesh->Render();
	}
}