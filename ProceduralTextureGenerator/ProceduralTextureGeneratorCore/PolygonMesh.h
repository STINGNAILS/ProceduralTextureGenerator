#pragma once

#include "DirectXDevice.h"


class PolygonMesh
{
	private:

	bool isInitialized;

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;

	UINT indexNum;

	UINT stride;
	UINT offset;

	public:

	PolygonMesh();
	~PolygonMesh();

	HRESULT Init(void *vertexData, UINT stride_, UINT vertexNum_, UINT *indexData, UINT indexNum_);

	void Render();
};

