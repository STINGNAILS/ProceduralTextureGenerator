#include "stdafx.h"
#include "RadianceMapRenderer.h"


RadianceMapRenderer::RadianceMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_)
{
	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	environmentMap = environmentMap_;
	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("RadianceMap");
	pixelShader = DirectXObjectPool::GetPixelShader("RadianceMap");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("Anisotropic");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("RadianceMap");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureRenderer");
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


HRESULT RadianceMapRenderer::Render(ID3D11Texture2D **radianceMap)
{
	HRESULT hr;

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

	samplerState->Set(0);
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