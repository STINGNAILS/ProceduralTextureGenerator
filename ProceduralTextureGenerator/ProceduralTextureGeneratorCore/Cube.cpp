#include "stdafx.h"
#include "Cube.h"


Cube::Cube()
{
	vertexShader = DirectXObjectPool::GetVertexShader("PBS");
	pixelShader = DirectXObjectPool::GetPixelShader("PBS");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("Basic");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Cube");
}


Cube::~Cube()
{

}


void Cube::SetBaseColorMap(shared_ptr<DirectXTexture> baseColorMap_)
{
	baseColorMap = baseColorMap_;
}


void Cube::SetMetallicMap(shared_ptr<DirectXTexture> metallicMap_)
{
	metallicMap = metallicMap_;
}


void Cube::SetRoughnessMap(shared_ptr<DirectXTexture> roughnessMap_)
{
	roughnessMap = roughnessMap_;
}


void Cube::SetNormalMap(shared_ptr<DirectXTexture> normalMap_)
{
	normalMap = normalMap_;
}


void Cube::Render()
{
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	samplerState->Set(1);
	baseColorMap->Set(2);
	metallicMap->Set(3);
	roughnessMap->Set(4);
	normalMap->Set(5);

	polygonMesh->Render();
}