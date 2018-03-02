#include "stdafx.h"
#include "Cube.h"


struct CubeVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 tex;
};


Cube::Cube()
{
	isInitialized = false;
}


Cube::~Cube()
{

}


HRESULT Cube::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("PBS");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = vertexShader->Init(L"PBS.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("PBS", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("PBS");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"PBS.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("PBS", pixelShader);
	}

	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	if(rasterizerState.get() == nullptr)
	{
		rasterizerState = make_shared<RasterizerState>();

		hr = rasterizerState->Init(D3D11_FILL_SOLID, D3D11_CULL_BACK);
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetRasterizerState("Basic", rasterizerState);
	}
	
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Cube");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<CubeVertex> cubeVertices(24);

		XMFLOAT3 v1;
		XMFLOAT3 v2;
		XMFLOAT3 v3;
		XMFLOAT3 v4;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;

		v1 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		v4 = XMFLOAT3(1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[0] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[1] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[2] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[3] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		v1 = XMFLOAT3(1.0f, 1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, 1.0f);
		v4 = XMFLOAT3(1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[4] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[5] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[6] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[7] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		v1 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(1.0f, -1.0f, -1.0f);
		v4 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[8] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[9] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[10] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[11] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		v4 = XMFLOAT3(-1.0f, -1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[12] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[13] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[14] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[15] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
		v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
		v3 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
		v4 = XMFLOAT3(1.0f, 1.0f, 1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[16] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[17] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[18] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[19] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		v1 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		v2 = XMFLOAT3(1.0f, -1.0f, 1.0f);
		v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		v4 = XMFLOAT3(1.0f, -1.0f, -1.0f);

		normal = CalculateNormal(v1, v2, v3);
		tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

		cubeVertices[20] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
		cubeVertices[21] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
		cubeVertices[22] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
		cubeVertices[23] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };


		vector<UINT> cubeIndices(36);

		for(int i = 0; i < 6; i++)
		{
			cubeIndices[6 * i + 0] = 4 * i + 0;
			cubeIndices[6 * i + 1] = 4 * i + 1;
			cubeIndices[6 * i + 2] = 4 * i + 2;
			cubeIndices[6 * i + 3] = 4 * i + 1;
			cubeIndices[6 * i + 4] = 4 * i + 3;
			cubeIndices[6 * i + 5] = 4 * i + 2;
		}


		hr = polygonMesh->Init((void*) &cubeVertices[0], sizeof(CubeVertex), cubeVertices.size(), &cubeIndices[0], cubeIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("Cube", polygonMesh);
	}

	isInitialized = true;

	return hr;
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
	if(isInitialized)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		baseColorMap->Set(2);
		metallicMap->Set(3);
		roughnessMap->Set(4);
		normalMap->Set(5);

		polygonMesh->Render();
	}
}