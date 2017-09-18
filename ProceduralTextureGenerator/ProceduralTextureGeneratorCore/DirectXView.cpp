#include "stdafx.h"
#include "DirectXView.h"


DirectXView::DirectXView()
{
	swapChain = NULL;
	renderTargetView = NULL;
	depthStencilView = NULL;
	depthStencilBuffer = NULL;
}


DirectXView::~DirectXView()
{

}

HRESULT DirectXView::Init(HWND hwnd, shared_ptr<DirectXDevice> device_)
{
	HRESULT hr = S_OK;

	device = device_;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	IDXGIDevice *dxgiDevice = nullptr;
	hr = device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if(FAILED(hr))
	{
		return hr;
	}

	IDXGIAdapter *dxgiAdapter = nullptr;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	if(FAILED(hr))
	{
		return hr;
	}

	IDXGIFactory *dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if(FAILED(hr))
	{
		return hr;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(device->GetDevice(), &swapChainDesc, &swapChain);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = Resize(hwnd);

	return hr;
}


HRESULT DirectXView::Resize(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	if(renderTargetView) renderTargetView->Release();
	if(depthStencilView) depthStencilView->Release();
	if(depthStencilBuffer) depthStencilBuffer->Release();

	hr = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if(FAILED(hr))
	{
		return hr;
	}

	ID3D11Texture2D *backBuffer = 0;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBuffer);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = device->GetDevice()->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();
	if(FAILED(hr))
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
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

	viewport.Width = (FLOAT) width;
	viewport.Height = (FLOAT) height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	return hr;
}


void DirectXView::BeginRender()
{
	const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	device->GetPainter()->ClearRenderTargetView(renderTargetView, clearColor);
	device->GetPainter()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	device->GetPainter()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	device->GetPainter()->RSSetViewports(1, &viewport);
}


void DirectXView::FinishRender()
{
	swapChain->Present(0, 0);
}


void DirectXView::Release()
{
	device = nullptr;

	if(swapChain) swapChain->Release();
	if(renderTargetView) renderTargetView->Release();
	if(depthStencilView) depthStencilView->Release();
	if(depthStencilBuffer) depthStencilBuffer->Release();
}
