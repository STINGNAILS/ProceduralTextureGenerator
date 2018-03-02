#pragma once

#include <memory>
#include "DirectXDevice.h"


using namespace std;


class DirectXView
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11Texture2D *viewBuffer;
	ID3D11Texture2D *depthStencilBuffer;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11DepthStencilState *depthStencilState;

	D3D11_VIEWPORT viewport;

	public:
	
	DirectXView();
	~DirectXView();

	HRESULT Init(void *viewResource);

	UINT GetWidth();
	UINT GetHeight();

	void BeginRender();
	void FinishRender();
};