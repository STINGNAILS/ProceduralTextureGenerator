#pragma once

#include <memory>
#include "DirectXDevice.h"


using namespace std;


class DirectXView
{
	private:

	bool isInitialized;

	shared_ptr<DirectXDevice> device;

	ID3D11Texture2D *viewBuffer;
	ID3D11Texture2D *depthStencilBuffer;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11DepthStencilState *depthStencilState;

	D3D11_VIEWPORT viewport;

	public:
	
	DirectXView();
	~DirectXView();

	HRESULT Init(shared_ptr<DirectXDevice> device_, void *viewResource);

	UINT GetWidth();
	UINT GetHeight();

	void BeginRender();
	void FinishRender();

	void Release();
};