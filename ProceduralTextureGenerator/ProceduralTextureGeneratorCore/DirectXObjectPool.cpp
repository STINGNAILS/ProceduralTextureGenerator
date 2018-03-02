#include "stdafx.h"
#include "DirectXObjectPool.h"


map<string, shared_ptr<PolygonMesh>> DirectXObjectPool::polygonMeshes;
map<string, shared_ptr<VertexShader>> DirectXObjectPool::vertexShaders;
map<string, shared_ptr<PixelShader>> DirectXObjectPool::pixelShaders;
map<string, shared_ptr<RasterizerState>> DirectXObjectPool::rasterizerStates;
map<string, shared_ptr<ConstantBuffer>> DirectXObjectPool::constantBuffers;


DirectXObjectPool::DirectXObjectPool()
{

}


DirectXObjectPool::~DirectXObjectPool()
{

}


void DirectXObjectPool::SetPolygonMesh(string key, shared_ptr<PolygonMesh> polygonMesh)
{
	if(polygonMesh.get() != nullptr)
	{
		polygonMeshes[key] = polygonMesh;
	}
}


void DirectXObjectPool::SetVertexShader(string key, shared_ptr<VertexShader> vertexShader)
{
	if(vertexShader.get() != nullptr)
	{
		vertexShaders[key] = vertexShader;
	}
}


void DirectXObjectPool::SetPixelShader(string key, shared_ptr<PixelShader> pixelShader)
{
	if(pixelShader.get() != nullptr)
	{
		pixelShaders[key] = pixelShader;
	}
}


void DirectXObjectPool::SetRasterizerState(string key, shared_ptr<RasterizerState> rasterizerState)
{
	if(rasterizerState.get() != nullptr)
	{
		rasterizerStates[key] = rasterizerState;
	}
}


void DirectXObjectPool::SetConstantBuffer(string key, shared_ptr<ConstantBuffer> constantBuffer)
{
	if(constantBuffer.get() != nullptr)
	{
		constantBuffers[key] = constantBuffer;
	}
}


shared_ptr<PolygonMesh> DirectXObjectPool::GetPolygonMesh(string key)
{
	auto it = polygonMeshes.find(key);

	if(it != polygonMeshes.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}


shared_ptr<VertexShader> DirectXObjectPool::GetVertexShader(string key)
{
	auto it = vertexShaders.find(key);

	if(it != vertexShaders.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}


shared_ptr<PixelShader> DirectXObjectPool::GetPixelShader(string key)
{
	auto it = pixelShaders.find(key);

	if(it != pixelShaders.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}


shared_ptr<RasterizerState> DirectXObjectPool::GetRasterizerState(string key)
{
	auto it = rasterizerStates.find(key);

	if(it != rasterizerStates.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}


shared_ptr<ConstantBuffer> DirectXObjectPool::GetConstantBuffer(string key)
{
	auto it = constantBuffers.find(key);

	if(it != constantBuffers.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
