#pragma once

#include "DirectXDevice.h"


class ConstantBuffer
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11Buffer *constantBuffer;

	public:

	ConstantBuffer(UINT byteWidth);
	~ConstantBuffer();

	void Update(void *cbData);
	void Set(int slot);
};

