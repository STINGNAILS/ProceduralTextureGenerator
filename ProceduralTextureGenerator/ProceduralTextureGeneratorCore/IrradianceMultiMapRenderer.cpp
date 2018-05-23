#include "stdafx.h"
#include "IrradianceMultiMapRenderer.h"


IrradianceMultiMapRenderer::IrradianceMultiMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_)
{
	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	environmentMap = environmentMap_;
	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("IrradianceMultiMap");
	pixelShader = DirectXObjectPool::GetPixelShader("IrradianceMultiMap");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("AnisotropicWrap");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("IrradianceMultiMap");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureRenderer");
}


IrradianceMultiMapRenderer::~IrradianceMultiMapRenderer()
{

}


HRESULT IrradianceMultiMapRenderer::Render(ID3D11Texture2D **irradianceMultiMap)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC irradianceMultiMapDesc;
	irradianceMultiMapDesc.Usage = D3D11_USAGE_DEFAULT;
	irradianceMultiMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	irradianceMultiMapDesc.CPUAccessFlags = 0;
	irradianceMultiMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	irradianceMultiMapDesc.Width = size;
	irradianceMultiMapDesc.Height = size;
	irradianceMultiMapDesc.MipLevels = 1;
	irradianceMultiMapDesc.ArraySize = 6;
	irradianceMultiMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	irradianceMultiMapDesc.SampleDesc.Count = 1;
	irradianceMultiMapDesc.SampleDesc.Quality = 0;

	hr = device->CreateTexture2D(&irradianceMultiMapDesc, 0, irradianceMultiMap);
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
		D3D11_RENDER_TARGET_VIEW_DESC irradianceMultiMapRTVDesc;
		ZeroMemory(&irradianceMultiMapRTVDesc, sizeof(irradianceMultiMapRTVDesc));
		irradianceMultiMapRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		irradianceMultiMapRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		irradianceMultiMapRTVDesc.Texture2DArray.MipSlice = 0;
		irradianceMultiMapRTVDesc.Texture2DArray.ArraySize = 1;
		irradianceMultiMapRTVDesc.Texture2DArray.FirstArraySlice = faceIndex;

		ID3D11RenderTargetView *irradianceMultiMapRTV;
		hr = device->CreateRenderTargetView(*irradianceMultiMap, &irradianceMultiMapRTVDesc, &irradianceMultiMapRTV);
		if(FAILED(hr))
		{
			return hr;
		}

		const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		painter->ClearRenderTargetView(irradianceMultiMapRTV, clearColor);
		painter->OMSetRenderTargets(1, &irradianceMultiMapRTV, nullptr);

		CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) size, (float) size);
		painter->RSSetViewports(1, &viewport);

		IrradianceMultiMapCB irradianceMultiMapCB;
		irradianceMultiMapCB.faceIndex = faceIndex;

		constantBuffer->Update(&irradianceMultiMapCB);

		polygonMesh->Render();

		irradianceMultiMapRTV->Release();
	}

	painter->RSSetState(0);
	painter->Flush();

	return hr;
}
