#include "stdafx.h"
#include "BackgroundGrid.h"


struct BackgroundGridVertex
{
	XMFLOAT3 pos;
};


BackgroundGrid::BackgroundGrid()
{
	vertexShader = DirectXObjectPool::GetVertexShader("BackgroundGrid");
	pixelShader = DirectXObjectPool::GetPixelShader("BackgroundGrid");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("Basic");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("BackgroundGrid");

	texture = make_shared<DirectXTexture>(BackgroundGridTexture());
}


BackgroundGrid::~BackgroundGrid()
{

}


void BackgroundGrid::Render()
{
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	samplerState->Set(0);
	texture->Set(0);

	polygonMesh->Render();
}
