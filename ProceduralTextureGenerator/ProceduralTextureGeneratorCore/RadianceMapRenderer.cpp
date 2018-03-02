#include "stdafx.h"
#include "RadianceMapRenderer.h"


struct RadianceMapVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};


RadianceMapRenderer::RadianceMapRenderer()
{
	isInitialized = false;

	size = 0;
}


RadianceMapRenderer::~RadianceMapRenderer()
{

}


int RadianceMapRenderer::MipLevelsNum(int size)
{
	switch(size)
	{
		case 128:
		{
			return 6;
		}
		case 256:
		{
			return 7;
		}
		case 512:
		{
			return 8;
		}
		case 1024:
		{
			return 9;
		}
		case 2048:
		{
			return 10;
		}
		case 4096:
		{
			return 11;
		}
		default:
		{
			return 2;
		}
	}
}


HRESULT RadianceMapRenderer::Init(shared_ptr<DirectXTexture> environmentMap_, int size_)
{
	HRESULT hr;

	isInitialized = false;

	if(!DirectXDevice::IsInitialized())
	{
		return E_FAIL;
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	environmentMap = environmentMap_;
	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("RadianceMap");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = vertexShader->Init(L"RadianceMapShader.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("RadianceMap", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("RadianceMap");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"RadianceMapShader.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("RadianceMap", pixelShader);
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

	constantBuffer = DirectXObjectPool::GetConstantBuffer("RadianceMap");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(RadianceMapCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("RadianceMap", constantBuffer);
	}

	polygonMesh = DirectXObjectPool::GetPolygonMesh("RadianceMap");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<RadianceMapVertex> radianceMapVertices(4);

		radianceMapVertices[0] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
		radianceMapVertices[1] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
		radianceMapVertices[2] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
		radianceMapVertices[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

		vector<UINT> radianceMapIndices(6);

		radianceMapIndices[0] = 0;
		radianceMapIndices[1] = 1;
		radianceMapIndices[2] = 2;
		radianceMapIndices[3] = 1;
		radianceMapIndices[4] = 3;
		radianceMapIndices[5] = 2;

		hr = polygonMesh->Init((void*) &radianceMapVertices[0], sizeof(RadianceMapVertex), radianceMapVertices.size(), &radianceMapIndices[0], radianceMapIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("RadianceMap", polygonMesh);
	}

	isInitialized = true;

	return hr;
}


HRESULT RadianceMapRenderer::Render(ID3D11Texture2D **radianceMap)
{
	HRESULT hr;

	if(!isInitialized)
	{
		return E_FAIL;
	}

	int mipLevelsNum = MipLevelsNum(size);

	D3D11_TEXTURE2D_DESC radianceMapDesc;
	radianceMapDesc.Usage = D3D11_USAGE_DEFAULT;
	radianceMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	radianceMapDesc.CPUAccessFlags = 0;
	radianceMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	radianceMapDesc.Width = size;
	radianceMapDesc.Height = size;
	radianceMapDesc.MipLevels = mipLevelsNum;
	radianceMapDesc.ArraySize = 6;
	radianceMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	radianceMapDesc.SampleDesc.Count = 1;
	radianceMapDesc.SampleDesc.Quality = 0;
	
	hr = device->CreateTexture2D(&radianceMapDesc, 0, radianceMap);
	if(FAILED(hr))
	{
		return hr;
	}


	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	constantBuffer->Set(0);

	environmentMap->Set(0);

	for(int faceIndex = 0; faceIndex < 6; faceIndex++)
	{
		int mipSize = size;

		for(int mipLevel = 0; mipLevel < mipLevelsNum; mipLevel++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC mipLevelRTVDesc;
			ZeroMemory(&mipLevelRTVDesc, sizeof(mipLevelRTVDesc));
			mipLevelRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			mipLevelRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			mipLevelRTVDesc.Texture2DArray.MipSlice = mipLevel;
			mipLevelRTVDesc.Texture2DArray.ArraySize = 1;
			mipLevelRTVDesc.Texture2DArray.FirstArraySlice = faceIndex;

			ID3D11RenderTargetView *mipLevelRTV;
			hr = device->CreateRenderTargetView(*radianceMap, &mipLevelRTVDesc, &mipLevelRTV);
			if(FAILED(hr))
			{
				return hr;
			}

			const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			painter->ClearRenderTargetView(mipLevelRTV, clearColor);
			painter->OMSetRenderTargets(1, &mipLevelRTV, nullptr);

			CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) mipSize, (float) mipSize);
			painter->RSSetViewports(1, &viewport);

			RadianceMapCB radianceMapCB;
			radianceMapCB.roughness = (float) mipLevel / (mipLevelsNum - 1);
			radianceMapCB.faceIndex = faceIndex;

			constantBuffer->Update(&radianceMapCB);

			polygonMesh->Render();


			mipLevelRTV->Release();
			mipSize /= 2;
		}
	}

	painter->RSSetState(0);

	return hr;
}