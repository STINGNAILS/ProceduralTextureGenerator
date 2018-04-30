 #include "stdafx.h"
#include "DirectXView.h"


DirectXView::DirectXView(void *viewResource)
{
	renderTargetView = nullptr;
	depthStencilView = nullptr;
	viewBuffer = nullptr;
	depthStencilBuffer = nullptr;
	depthStencilState = nullptr;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	IUnknown *unknownResource = (IUnknown*) viewResource;

	IDXGIResource *dxgiResource;
	hr = unknownResource->QueryInterface(__uuidof(IDXGIResource), (void**) (&dxgiResource));
	if(FAILED(hr))
	{
		throw "Couldn't query view resource as DXGI";
	}

	HANDLE sharedHandle;
	hr = dxgiResource->GetSharedHandle(&sharedHandle);
	dxgiResource->Release();
	if(FAILED(hr))
	{
		throw "Couldn't get shared handle of DXGI resource";
	}


	IUnknown *tempResource;
	hr = device->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**) (&tempResource));
	if(FAILED(hr))
	{
		throw "Couldn't open shared resource";
	}

	hr = tempResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**) (&viewBuffer));
	tempResource->Release();
	if(FAILED(hr))
	{
		throw "Couldn't query shared resource as texture";
	}

	IDXGISurface *dxgiSurface;
	hr = viewBuffer->QueryInterface(__uuidof(IDXGISurface), (void**) (&dxgiSurface));
	if(FAILED(hr))
	{
		throw "Couldn't query texture as DXGI surface";
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(viewBuffer, &rtvDesc, &renderTargetView);
	if(FAILED(hr))
	{
		throw "Couldn't create render target view";
	}

	D3D11_TEXTURE2D_DESC viewDesc;
	viewBuffer->GetDesc(&viewDesc);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
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

	hr = device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	if(FAILED(hr))
	{
		throw "Couldn't create depth stencil buffer";
	}

	hr = device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	if(FAILED(hr))
	{
		throw "Couldn't create depth stencil view";
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
	if(FAILED(hr))
	{
		throw "Couldn't create depth stencil state";
	}

	viewport.Width = (float) viewDesc.Width;
	viewport.Height = (float) viewDesc.Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
}


DirectXView::~DirectXView()
{
	if(depthStencilState) depthStencilState->Release();
	if(viewBuffer) viewBuffer->Release();
	if(depthStencilBuffer) depthStencilBuffer->Release();
	if(renderTargetView) renderTargetView->Release();
	if(depthStencilView) depthStencilView->Release();
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
	const float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	painter->ClearRenderTargetView(renderTargetView, clearColor);
	painter->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	painter->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	painter->OMSetDepthStencilState(depthStencilState, 1);
	painter->RSSetViewports(1, &viewport);
}


void DirectXView::FinishRender()
{
	painter->Flush();
}