#include "stdafx.h"
#include "IrradianceRoughMapRenderer.h"


IrradianceRoughMapRenderer::IrradianceRoughMapRenderer(shared_ptr<DirectXTexture> environmentMap_, int size_)
{
	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	environmentMap = environmentMap_;
	size = size_;

	vertexShader = DirectXObjectPool::GetVertexShader("IrradianceRoughMap");
	pixelShader = DirectXObjectPool::GetPixelShader("IrradianceRoughMap");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("AnisotropicWrap");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("IrradianceRoughMap");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureRenderer");
}


IrradianceRoughMapRenderer::~IrradianceRoughMapRenderer()
{

}


HRESULT IrradianceRoughMapRenderer::Render(ID3D11Texture2D **irradianceRoughMap)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC irradianceRoughMapDesc;
	irradianceRoughMapDesc.Usage = D3D11_USAGE_DEFAULT;
	irradianceRoughMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	irradianceRoughMapDesc.CPUAccessFlags = 0;
	irradianceRoughMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	irradianceRoughMapDesc.Width = size;
	irradianceRoughMapDesc.Height = size;
	irradianceRoughMapDesc.MipLevels = 1;
	irradianceRoughMapDesc.ArraySize = 6;
	irradianceRoughMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	irradianceRoughMapDesc.SampleDesc.Count = 1;
	irradianceRoughMapDesc.SampleDesc.Quality = 0;

	hr = device->CreateTexture2D(&irradianceRoughMapDesc, 0, irradianceRoughMap);
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
		D3D11_RENDER_TARGET_VIEW_DESC irradianceRoughMapRTVDesc;
		ZeroMemory(&irradianceRoughMapRTVDesc, sizeof(irradianceRoughMapRTVDesc));
		irradianceRoughMapRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		irradianceRoughMapRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		irradianceRoughMapRTVDesc.Texture2DArray.MipSlice = 0;
		irradianceRoughMapRTVDesc.Texture2DArray.ArraySize = 1;
		irradianceRoughMapRTVDesc.Texture2DArray.FirstArraySlice = faceIndex;

		ID3D11RenderTargetView *irradianceRoughMapRTV;
		hr = device->CreateRenderTargetView(*irradianceRoughMap, &irradianceRoughMapRTVDesc, &irradianceRoughMapRTV);
		if(FAILED(hr))
		{
			return hr;
		}

		const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		painter->ClearRenderTargetView(irradianceRoughMapRTV, clearColor);
		painter->OMSetRenderTargets(1, &irradianceRoughMapRTV, nullptr);

		CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) size, (float) size);
		painter->RSSetViewports(1, &viewport);

		IrradianceRoughMapCB irradianceRoughMapCB;
		irradianceRoughMapCB.faceIndex = faceIndex;

		constantBuffer->Update(&irradianceRoughMapCB);

		polygonMesh->Render();

		irradianceRoughMapRTV->Release();
	}

	painter->RSSetState(0);
	painter->Flush();

	return hr;
}
