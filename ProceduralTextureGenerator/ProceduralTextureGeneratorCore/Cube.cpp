#include "stdafx.h"
#include "Cube.h"


struct CubeVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 tex;
};


Cube::Cube(shared_ptr<DirectXDevice> device_)
{
	device = device_;

	fxIsInitialized = false;
	geometryIsInitialized = false;
}


Cube::~Cube()
{
	Release();
}


HRESULT Cube::Init()
{
	HRESULT hr = S_OK;

	if(!fxIsInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"PBS.fx", "VS", "vs_5_0", &shaderBlob);
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
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"PBS.fx", "PS", "ps_5_0", &shaderBlob);
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
	}

	if(!geometryIsInitialized)
	{
		vector<CubeVertex> cubeVertices(36);

		XMFLOAT3 v1;
		XMFLOAT3 v2;
		XMFLOAT3 v3;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;


		v1 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[0].pos = v1;
		cubeVertices[0].normal = normal;
		cubeVertices[0].tangent = tangent;
		cubeVertices[0].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[1].pos = v2;
		cubeVertices[1].normal = normal;
		cubeVertices[1].tangent = tangent;
		cubeVertices[1].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[2].pos = v3;
		cubeVertices[2].normal = normal;
		cubeVertices[2].tangent = tangent;
		cubeVertices[2].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, -1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[3].pos = v1;
		cubeVertices[3].normal = normal;
		cubeVertices[3].tangent = tangent;
		cubeVertices[3].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[4].pos = v2;
		cubeVertices[4].normal = normal;
		cubeVertices[4].tangent = tangent;
		cubeVertices[4].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[5].pos = v3;
		cubeVertices[5].normal = normal;
		cubeVertices[5].tangent = tangent;
		cubeVertices[5].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[6].pos = v1;
		cubeVertices[6].normal = normal;
		cubeVertices[6].tangent = tangent;
		cubeVertices[6].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[7].pos = v2;
		cubeVertices[7].normal = normal;
		cubeVertices[7].tangent = tangent;
		cubeVertices[7].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[8].pos = v3;
		cubeVertices[8].normal = normal;
		cubeVertices[8].tangent = tangent;
		cubeVertices[8].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(1.0f, -1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[9].pos = v1;
		cubeVertices[9].normal = normal;
		cubeVertices[9].tangent = tangent;
		cubeVertices[9].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[10].pos = v2;
		cubeVertices[10].normal = normal;
		cubeVertices[10].tangent = tangent;
		cubeVertices[10].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[11].pos = v3;
		cubeVertices[11].normal = normal;
		cubeVertices[11].tangent = tangent;
		cubeVertices[11].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[12].pos = v1;
		cubeVertices[12].normal = normal;
		cubeVertices[12].tangent = tangent;
		cubeVertices[12].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[13].pos = v2;
		cubeVertices[13].normal = normal;
		cubeVertices[13].tangent = tangent;
		cubeVertices[13].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[14].pos = v3;
		cubeVertices[14].normal = normal;
		cubeVertices[14].tangent = tangent;
		cubeVertices[14].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[15].pos = v1;
		cubeVertices[15].normal = normal;
		cubeVertices[15].tangent = tangent;
		cubeVertices[15].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[16].pos = v2;
		cubeVertices[16].normal = normal;
		cubeVertices[16].tangent = tangent;
		cubeVertices[16].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[17].pos = v3;
		cubeVertices[17].normal = normal;
		cubeVertices[17].tangent = tangent;
		cubeVertices[17].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[18].pos = v1;
		cubeVertices[18].normal = normal;
		cubeVertices[18].tangent = tangent;
		cubeVertices[18].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[19].pos = v2;
		cubeVertices[19].normal = normal;
		cubeVertices[19].tangent = tangent;
		cubeVertices[19].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[20].pos = v3;
		cubeVertices[20].normal = normal;
		cubeVertices[20].tangent = tangent;
		cubeVertices[20].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[21].pos = v1;
		cubeVertices[21].normal = normal;
		cubeVertices[21].tangent = tangent;
		cubeVertices[21].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[22].pos = v2;
		cubeVertices[22].normal = normal;
		cubeVertices[22].tangent = tangent;
		cubeVertices[22].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[23].pos = v3;
		cubeVertices[23].normal = normal;
		cubeVertices[23].tangent = tangent;
		cubeVertices[23].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(-1.0f, 1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[24].pos = v1;
		cubeVertices[24].normal = normal;
		cubeVertices[24].tangent = tangent;
		cubeVertices[24].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[25].pos = v2;
		cubeVertices[25].normal = normal;
		cubeVertices[25].tangent = tangent;
		cubeVertices[25].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[26].pos = v3;
		cubeVertices[26].normal = normal;
		cubeVertices[26].tangent = tangent;
		cubeVertices[26].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, 1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[27].pos = v1;
		cubeVertices[27].normal = normal;
		cubeVertices[27].tangent = tangent;
		cubeVertices[27].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[28].pos = v2;
		cubeVertices[28].normal = normal;
		cubeVertices[28].tangent = tangent;
		cubeVertices[28].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[29].pos = v3;
		cubeVertices[29].normal = normal;
		cubeVertices[29].tangent = tangent;
		cubeVertices[29].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, -1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[30].pos = v1;
		cubeVertices[30].normal = normal;
		cubeVertices[30].tangent = tangent;
		cubeVertices[30].tex = XMFLOAT2(0.0f, 0.0f);

		cubeVertices[31].pos = v2;
		cubeVertices[31].normal = normal;
		cubeVertices[31].tangent = tangent;
		cubeVertices[31].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[32].pos = v3;
		cubeVertices[32].normal = normal;
		cubeVertices[32].tangent = tangent;
		cubeVertices[32].tex = XMFLOAT2(0.0f, 1.0f);


		v1 = XMFLOAT3(1.0f, -1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, -1.0f, -1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[33].pos = v1;
		cubeVertices[33].normal = normal;
		cubeVertices[33].tangent = tangent;
		cubeVertices[33].tex = XMFLOAT2(1.0f, 0.0f);

		cubeVertices[34].pos = v2;
		cubeVertices[34].normal = normal;
		cubeVertices[34].tangent = tangent;
		cubeVertices[34].tex = XMFLOAT2(1.0f, 1.0f);

		cubeVertices[35].pos = v3;
		cubeVertices[35].normal = normal;
		cubeVertices[35].tangent = tangent;
		cubeVertices[35].tex = XMFLOAT2(0.0f, 1.0f);


		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = sizeof(CubeVertex) * cubeVertices.size();
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
	}
}


void Cube::SetBaseColorMap(shared_ptr<DirectXTexture> baseColorMap_)
{
	baseColorMap = baseColorMap_;
}


void Cube::SetMetallicMap(shared_ptr<DirectXTexture> metallicMap_)
{
	metallicMap = metallicMap_;
}


void Cube::SetRoughnessMap(shared_ptr<DirectXTexture> roughnessMap_)
{
	roughnessMap = roughnessMap_;
}


void Cube::SetNormalMap(shared_ptr<DirectXTexture> normalMap_)
{
	normalMap = normalMap_;
}


void Cube::Render()
{
	UINT stride = sizeof(CubeVertex);
	UINT offset = 0;

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(basicRasterizerState);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);

	device->GetPainter()->PSSetShader(pixelShader, 0, 0);

	baseColorMap->Set(2);
	metallicMap->Set(3);
	roughnessMap->Set(4);
	normalMap->Set(5);

	device->GetPainter()->Draw(36, 0);

	device->GetPainter()->RSSetState(0);
}


void Cube::ReleaseFX()
{
	fxIsInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}


void Cube::ReleaseGeometry()
{
	geometryIsInitialized = false;

	if(vertexBuffer) vertexBuffer->Release();
}

void Cube::ReleaseTextures()
{
	baseColorMap = nullptr;
	metallicMap = nullptr;
	roughnessMap = nullptr;
	normalMap = nullptr;
}


void Cube::Release()
{
	device = nullptr;

	ReleaseTextures();
	ReleaseGeometry();
	ReleaseFX();
}
