#pragma once

#include "DirectXDevice.h"


class PolygonMesh
{
	private:

	ID3D11Device *device;
	ID3D11DeviceContext *painter;

	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;

	UINT indexNum;

	UINT stride;
	UINT offset;

	public:

	PolygonMesh(void *vertexData, UINT stride_, UINT vertexNum_, UINT *indexData, UINT indexNum_);
	~PolygonMesh();

	void Render();
};

