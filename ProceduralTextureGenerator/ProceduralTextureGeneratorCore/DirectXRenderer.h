#pragma once

#include "DirectXDevice.h"
#include "DirectXObjectPool.h"

class DirectXRenderer
{
	protected:

	shared_ptr<DirectXDevice> device;

	public:

	DirectXRenderer();
	~DirectXRenderer();

	virtual HRESULT Render(ID3D11Texture2D **texture) = 0;
};

