#include "stdafx.h"
#include "Environment.h"


struct CubeMapVertex
{
	XMFLOAT3 pos;
};


Environment::Environment(shared_ptr<DirectXDevice> device_)
{
	device = device_;

	lastDirectionalLightNum = 0;
	lastPunctualLightNum = 0;

	fxIsInitialized = false;
	geometryIsInitialized = false;
	cubeMapIsInitialized = false;
}


Environment::~Environment()
{
	Release();
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


HRESULT Environment::Init(LPCWSTR fileName)
{
	HRESULT hr = S_OK;

	if(!fxIsInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"EnvironmentShader.fx", "VS", "vs_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
		if(FAILED(hr))
		{
			shaderBlob->Release();
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"EnvironmentShader.fx", "PS", "ps_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(EnvironmentCB);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &environmentConstantBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;

		hr = device->GetDevice()->CreateRasterizerState(&rasterizerDesc, &basicRasterizerState);
		if(FAILED(hr))
		{
			return hr;
		}

		fxIsInitialized = true;
	}

	if(!geometryIsInitialized)
	{
		vector<CubeMapVertex> cubeVertices(36);


		cubeVertices[0].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[1].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		cubeVertices[2].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);

		cubeVertices[3].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		cubeVertices[4].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		cubeVertices[5].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);


		cubeVertices[6].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
		cubeVertices[7].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[8].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);

		cubeVertices[9].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[10].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
		cubeVertices[11].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);


		cubeVertices[12].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		cubeVertices[13].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
		cubeVertices[14].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		cubeVertices[15].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
		cubeVertices[16].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
		cubeVertices[17].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);


		cubeVertices[18].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		cubeVertices[19].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		cubeVertices[20].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		cubeVertices[21].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		cubeVertices[22].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		cubeVertices[23].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);


		cubeVertices[24].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		cubeVertices[25].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[26].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);

		cubeVertices[27].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cubeVertices[28].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
		cubeVertices[29].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);


		cubeVertices[30].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		cubeVertices[31].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
		cubeVertices[32].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		cubeVertices[33].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
		cubeVertices[34].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
		cubeVertices[35].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);


		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = sizeof(CubeMapVertex) * cubeVertices.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = &cubeVertices[0];

		hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		geometryIsInitialized = true;
	}

	ReleaseCubeMap();

	cubeMap = make_shared<DirectXTexture>(device);
	hr = cubeMap->InitFromFile(fileName);
	if(FAILED(hr))
	{
		return hr;
	}

	shared_ptr<RadianceMapRenderer> radianceMapRenderer = make_shared<RadianceMapRenderer>(device);
	hr = radianceMapRenderer->Init(cubeMap, 2048);
	if(FAILED(hr))
	{
		return hr;
	}

	radianceMap = make_shared<DirectXTexture>(device);
	hr = radianceMap->InitFromRenderer(radianceMapRenderer);
	if(FAILED(hr))
	{
		return hr;
	}

	shared_ptr<BRDFLUTRenderer> brdfLUTRenderer = make_shared<BRDFLUTRenderer>(device);
	hr = brdfLUTRenderer->Init(2048);
	if(FAILED(hr))
	{
		return hr;
	}

	brdfLUT = make_shared<DirectXTexture>(device);
	hr = brdfLUT->InitFromRenderer(brdfLUTRenderer);
	if(FAILED(hr))
	{
		return hr;
	}

	cubeMapIsInitialized = true;

	return hr;
}


void Environment::Prepare()
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

	device->GetPainter()->UpdateSubresource(environmentConstantBuffer, 0, 0, &environmentCB, 0, 0);

	device->GetPainter()->VSSetConstantBuffers(1, 1, &environmentConstantBuffer);
	device->GetPainter()->PSSetConstantBuffers(1, 1, &environmentConstantBuffer);

	radianceMap->Set(0);
	brdfLUT->Set(1);
}


void Environment::Render()
{
	UINT stride = sizeof(CubeMapVertex);
	UINT offset = 0;

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(basicRasterizerState);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);

	device->GetPainter()->PSSetShader(pixelShader, 0, 0);

	cubeMap->Set(0);

	device->GetPainter()->Draw(36, 0);

	device->GetPainter()->RSSetState(0);
}


void Environment::ReleaseFX()
{
	if(inputLayout) inputLayout->Release();
	if(environmentConstantBuffer) environmentConstantBuffer->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}


void Environment::ReleaseGeometry()
{
	if(vertexBuffer) vertexBuffer->Release();
}


void Environment::ReleaseCubeMap()
{
	cubeMap = nullptr;
	radianceMap = nullptr;
	brdfLUT = nullptr;
}


void Environment::Release()
{
	device = nullptr;

	ReleaseCubeMap();
	ReleaseGeometry();
	ReleaseFX();
}