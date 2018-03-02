#pragma once

#include "DirectXDevice.h"


class ConstantBuffer
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11Buffer *constantBuffer;

	public:

	ConstantBuffer();
	~ConstantBuffer();

	HRESULT Init(UINT byteWidth);

	void Update(void *cbData);
	void Set(int slot);
};

