#include "stdafx.h"
#include "BRDFLUTRenderer.h"


BRDFLUTRenderer::BRDFLUTRenderer(int size_)
{
	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("BRDFLUT");
	pixelShader = DirectXObjectPool::GetPixelShader("BRDFLUT");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureRenderer");
}


BRDFLUTRenderer::~BRDFLUTRenderer()
{

}


HRESULT BRDFLUTRenderer::Render(ID3D11Texture2D **brdfLUT)
{
	HRESULT hr;

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