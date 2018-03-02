#include "stdafx.h"
#include "BRDFLUTRenderer.h"


struct BRDFLUTVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};



BRDFLUTRenderer::BRDFLUTRenderer()
{
	isInitialized = false;

	size = 0;
}


BRDFLUTRenderer::~BRDFLUTRenderer()
{

}


HRESULT BRDFLUTRenderer::Init(int size_)
{
	HRESULT hr;

	isInitialized = false;

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("BRDFLUT");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = vertexShader->Init(L"BRDFLUTShader.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("BRDFLUT", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("BRDFLUT");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"BRDFLUTShader.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("BRDFLUT", pixelShader);
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
	
	polygonMesh = DirectXObjectPool::GetPolygonMesh("BRDFLUT");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<BRDFLUTVertex> brdfLUTVertices(4);

		brdfLUTVertices[0] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
		brdfLUTVertices[1] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
		brdfLUTVertices[2] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
		brdfLUTVertices[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

		vector<UINT> brdfLUTIndices(6);

		brdfLUTIndices[0] = 0;
		brdfLUTIndices[1] = 1;
		brdfLUTIndices[2] = 2;
		brdfLUTIndices[3] = 1;
		brdfLUTIndices[4] = 3;
		brdfLUTIndices[5] = 2;

		hr = polygonMesh->Init((void*) &brdfLUTVertices[0], sizeof(BRDFLUTVertex), brdfLUTVertices.size(), &brdfLUTIndices[0], brdfLUTIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("BRDFLUT", polygonMesh);
	}

	isInitialized = true;

	return hr;
}


HRESULT BRDFLUTRenderer::Render(ID3D11Texture2D **brdfLUT)
{
	HRESULT hr;

	if(!isInitialized)
	{
		return E_FAIL;
	}

	D3D11_TEXTURE2D_DESC brdfLUTDesc;
	brdfLUTDesc.Usage = D3D11_USAGE_DEFAULT;
	brdfLUTDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	brdfLUTDesc.CPUAccessFlags = 0;
	brdfLUTDesc.MiscFlags = 0;
	brdfLUTDesc.Width = size;
	brdfLUTDesc.Height = size;
	brdfLUTDesc.MipLevels = 1;
	brdfLUTDesc.ArraySize = 1;
	brdfLUTDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	brdfLUTDesc.SampleDesc.Count = 1;
	brdfLUTDesc.SampleDesc.Quality = 0;

	hr = device->CreateTexture2D(&brdfLUTDesc, 0, brdfLUT);
	if(FAILED(hr))
	{
		return hr;
	}


	D3D11_RENDER_TARGET_VIEW_DESC brdfLUTRTVDesc;
	ZeroMemory(&brdfLUTRTVDesc, sizeof(brdfLUTRTVDesc));
	brdfLUTRTVDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	brdfLUTRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	brdfLUTRTVDesc.Texture2DArray.MipSlice = 0;

	ID3D11RenderTargetView *mipLevelRTV;
	hr = device->CreateRenderTargetView(*brdfLUT, &brdfLUTRTVDesc, &mipLevelRTV);
	if(FAILED(hr))
	{
		return hr;
	}

	const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	painter->ClearRenderTargetView(mipLevelRTV, clearColor);
	painter->OMSetRenderTargets(1, &mipLevelRTV, nullptr);

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) size, (float) size);
	painter->RSSetViewports(1, &viewport);

	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	polygonMesh->Render();

	mipLevelRTV->Release();

	return hr;
}