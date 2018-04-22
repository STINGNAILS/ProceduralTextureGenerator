#include "stdafx.h"
#include "BackgroundGrid.h"


struct BackgroundGridVertex
{
	XMFLOAT3 pos;
};


BackgroundGrid::BackgroundGrid()
{
	isInitialized = false;
}


BackgroundGrid::~BackgroundGrid()
{

}


HRESULT BackgroundGrid::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("BackgroundGrid");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = vertexShader->Init(L"BackgroundGrid.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("BackgroundGrid", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("BackgroundGrid");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"BackgroundGrid.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("BackgroundGrid", pixelShader);
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

	samplerState = DirectXObjectPool::GetSamplerState("Basic");
	if(rasterizerState.get() == nullptr)
	{
		samplerState = make_shared<SamplerState>();

		hr = samplerState->Init(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0);
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetSamplerState("Basic", samplerState);
	}

	polygonMesh = DirectXObjectPool::GetPolygonMesh("BackgroundGrid");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<BackgroundGridVertex> backgroundGridVertices(4);

		backgroundGridVertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f) };
		backgroundGridVertices[1] = { XMFLOAT3(1.0f, -1.0f, 0.0f) };
		backgroundGridVertices[2] = { XMFLOAT3(-1.0f, 1.0f, 0.0f) };
		backgroundGridVertices[3] = { XMFLOAT3(1.0f, 1.0f, 0.0f) };

		vector<UINT> backgroundGridIndices(6);

		backgroundGridIndices[0] = 0;
		backgroundGridIndices[1] = 2;
		backgroundGridIndices[2] = 3;
		backgroundGridIndices[3] = 0;
		backgroundGridIndices[4] = 3;
		backgroundGridIndices[5] = 1;

		hr = polygonMesh->Init((void*) &backgroundGridVertices[0], sizeof(BackgroundGridVertex), backgroundGridVertices.size(), &backgroundGridIndices[0], backgroundGridIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("BackgroundGrid", polygonMesh);
	}

	texture = make_shared<DirectXTexture>();
	texture->InitFromMemory(BackgroundGridTexture());

	isInitialized = true;

	return hr;
}


void BackgroundGrid::Render()
{
	if(isInitialized)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		samplerState->Set(0);
		texture->Set(0);

		polygonMesh->Render();
	}
}
