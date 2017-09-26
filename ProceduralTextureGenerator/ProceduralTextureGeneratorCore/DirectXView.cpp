#include "stdafx.h"
#include "DirectXView.h"


DirectXView::DirectXView()
{
	//swapChain = NULL;
	renderTargetView = nullptr;
	depthStencilView = nullptr;
	viewBuffer = nullptr;
	depthStencilBuffer = nullptr;
	depthStencilState = nullptr;

	isInitialized = false;
}


DirectXView::~DirectXView()
{
	Release();
}


HRESULT DirectXView::Init(shared_ptr<DirectXDevice> device_, void *viewResource)
{
	HRESULT hr = S_OK;

	Release();
	device = device_;

	IUnknown *unknownResource = (IUnknown*) viewResource;

	IDXGIResource *dxgiResource;
	hr = unknownResource->QueryInterface(__uuidof(IDXGIResource), (void**) &dxgiResource);
	if(FAILED(hr))
	{
		return hr;
	}

	HANDLE sharedHandle;
	hr = dxgiResource->GetSharedHandle(&sharedHandle);
	if(FAILED(hr))
	{
		return hr;
	}

	dxgiResource->Release();

	IUnknown *tempResource;
	hr = device->GetDevice()->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**) (&tempResource));
	if(FAILED(hr))
	{
		return hr;
	}

	hr = tempResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**) (&viewBuffer));
	if(FAILED(hr))
	{
		return hr;
	}

	tempResource->Release();

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->GetDevice()->CreateRenderTargetView(viewBuffer, &rtvDesc, &renderTargetView);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC viewDesc;
	viewBuffer->GetDesc(&viewDesc);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.Width = viewDesc.Width;
	depthStencilDesc.Height = viewDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	hr = device->GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = device->GetDevice()->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
	if(FAILED(hr))
	{
		return hr;
	}

	viewport.Width = (float) viewDesc.Width;
	viewport.Height = (float) viewDesc.Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	isInitialized = true;

	return hr;
}


UINT DirectXView::GetWidth()
{
	return viewport.Width;
}


UINT DirectXView::GetHeight()
{
	return viewport.Height;
}


void DirectXView::BeginRender()
{
	if(isInitialized)
	{
		const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		device->GetPainter()->ClearRenderTargetView(renderTargetView, clearColor);
		device->GetPainter()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		device->GetPainter()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		device->GetPainter()->OMSetDepthStencilState(depthStencilState, 1);
		device->GetPainter()->RSSetViewports(1, &viewport);
	}
}


void DirectXView::FinishRender()
{
	if(isInitialized)
	{
		device->GetPainter()->Flush();
	}
}


void DirectXView::Release()
{
	device = nullptr;

	if(depthStencilState) depthStencilState->Release();
	if(viewBuffer) viewBuffer->Release();
	if(depthStencilBuffer) depthStencilBuffer->Release();
	if(renderTargetView) renderTargetView->Release();
	if(depthStencilView) depthStencilView->Release();
}
