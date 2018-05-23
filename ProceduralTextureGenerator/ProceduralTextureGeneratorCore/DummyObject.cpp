#include "stdafx.h"
#include "DummyObject.h"


DummyObject::DummyObject(string meshType)
{
	vertexShader = DirectXObjectPool::GetVertexShader("PBS");
	pixelShader = DirectXObjectPool::GetPixelShader("PBS");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("LinearWrap");
	polygonMesh = DirectXObjectPool::GetPolygonMesh(meshType);
}


DummyObject::~DummyObject()
{

}


void DummyObject::SetBaseColorMap(shared_ptr<DirectXTexture> baseColorMap_)
{
	baseColorMap = baseColorMap_;
}


void DummyObject::SetMetallicMap(shared_ptr<DirectXTexture> metallicMap_)
{
	metallicMap = metallicMap_;
}


void DummyObject::SetRoughnessMap(shared_ptr<DirectXTexture> roughnessMap_)
{
	roughnessMap = roughnessMap_;
}


void DummyObject::SetNormalMap(shared_ptr<DirectXTexture> normalMap_)
{
	normalMap = normalMap_;
}


void DummyObject::Render()
{
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	samplerState->Set(2);

	baseColorMap->Set(5);
	metallicMap->Set(6);
	roughnessMap->Set(7);
	normalMap->Set(8);

	polygonMesh->Render();
}