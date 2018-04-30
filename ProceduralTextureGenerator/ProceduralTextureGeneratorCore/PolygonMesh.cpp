#include "stdafx.h"
#include "PolygonMesh.h"


PolygonMesh::PolygonMesh(void *vertexData, UINT stride_, UINT vertexNum_, UINT *indexData, UINT indexNum_)
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	indexNum = 0;

	if(!DirectXDevice::IsInitialized())
	{
		throw "Device wasn't initialized";
	}

	device = DirectXDevice::GetDevice();
	painter = DirectXDevice::GetPainter();

	HRESULT hr = S_OK;

	indexNum = indexNum_;

	stride = stride_;
	offset = 0;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = stride * vertexNum_;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = vertexData;

	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if(FAILED(hr))
	{
		throw "Couldn't create vertex buffer";
	}

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ibDesc.ByteWidth = sizeof(UINT) * indexNum;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = indexData;

	hr = device->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
	if(FAILED(hr))
	{
		throw "Couldn't create index buffer";
	}
}


PolygonMesh::~PolygonMesh()
{
	if(vertexBuffer) vertexBuffer->Release();
	if(indexBuffer) indexBuffer->Release();
}


void PolygonMesh::Render()
{
	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	painter->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	painter->DrawIndexed(indexNum, 0, 0);
}
