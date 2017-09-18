#pragma once

#include <memory>
#include "DirectXDevice.h"


using namespace std;


class DirectXView
{
	private:

	shared_ptr<DirectXDevice> device;

	IDXGISwapChain *swapChain;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11Texture2D *depthStencilBuffer;

	D3D11_VIEWPORT viewport;

	public:
	
	DirectXView();
	~DirectXView();

	HRESULT Init(HWND hwnd, shared_ptr<DirectXDevice> device_);
	HRESULT Resize(HWND hwnd);

	void BeginRender();
	void FinishRender();

	void Release();
};