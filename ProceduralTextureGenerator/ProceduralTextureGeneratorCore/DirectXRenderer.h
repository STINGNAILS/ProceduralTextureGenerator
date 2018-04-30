#pragma once

#include "DirectXDevice.h"
#include "DirectXObjectPool.h"

class DirectXRenderer
{
	public:

	DirectXRenderer();
	~DirectXRenderer();

	virtual HRESULT Render(ID3D11Texture2D **texture) = 0;
};

