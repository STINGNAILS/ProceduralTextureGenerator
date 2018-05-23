#include "stdafx.h"
#include "IrradianceSmoothMapRenderer.h"


IrradianceSmoothMapRenderer::IrradianceSmoothMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_)
{
	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	environmentMap = environmentMap_;
	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("IrradianceSmoothMap");
	pixelShader = DirectXObjectPool::GetPixelShader("IrradianceSmoothMap");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("AnisotropicWrap");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("IrradianceSmoothMap");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureRenderer");
}


IrradianceSmoothMapRenderer::~IrradianceSmoothMapRenderer()
{

}


HRESULT IrradianceSmoothMapRenderer::Render(ID3D11Texture2D **irradianceSmoothMap)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC irradianceSmoothMapDesc;
	irradianceSmoothMapDesc.Usage = D3D11_USAGE_DEFAULT;
	irradianceSmoothMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	irradianceSmoothMapDesc.CPUAccessFlags = 0;
	irradianceSmoothMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	irradianceSmoothMapDesc.Width = size;
	irradianceSmoothMapDesc.Height = size;
	irradianceSmoothMapDesc.MipLevels = 1;
	irradianceSmoothMapDesc.ArraySize = 6;
	irradianceSmoothMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	irradianceSmoothMapDesc.SampleDesc.Count = 1;
	irradianceSmoothMapDesc.SampleDesc.Quality = 0;

	hr = device->CreateTexture2D(&irradianceSmoothMapDesc, 0, irradianceSmoothMap);
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
		D3D11_RENDER_TARGET_VIEW_DESC irradianceSmoothMapRTVDesc;
		ZeroMemory(&irradianceSmoothMapRTVDesc, sizeof(irradianceSmoothMapRTVDesc));
		irradianceSmoothMapRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		irradianceSmoothMapRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		irradianceSmoothMapRTVDesc.Texture2DArray.MipSlice = 0;
		irradianceSmoothMapRTVDesc.Texture2DArray.ArraySize = 1;
		irradianceSmoothMapRTVDesc.Texture2DArray.FirstArraySlice = faceIndex;

		ID3D11RenderTargetView *irradianceSmoothMapRTV;
		hr = device->CreateRenderTargetView(*irradianceSmoothMap, &irradianceSmoothMapRTVDesc, &irradianceSmoothMapRTV);
		if(FAILED(hr))
		{
			return hr;
		}

		const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		painter->ClearRenderTargetView(irradianceSmoothMapRTV, clearColor);
		painter->OMSetRenderTargets(1, &irradianceSmoothMapRTV, nullptr);

		CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) size, (float) size);
		painter->RSSetViewports(1, &viewport);

		IrradianceSmoothMapCB irradianceSmoothMapCB;
		irradianceSmoothMapCB.faceIndex = faceIndex;

		constantBuffer->Update(&irradianceSmoothMapCB);

		polygonMesh->Render();

		irradianceSmoothMapRTV->Release();
	}

	painter->RSSetState(0);
	painter->Flush();

	return hr;
}
