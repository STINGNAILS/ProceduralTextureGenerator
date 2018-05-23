#pragma once

#include "PolygonMesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"


class DirectXObjectPool
{
	private:

	static map<string, shared_ptr<PolygonMesh>> polygonMeshes;
	static map<string, shared_ptr<VertexShader>> vertexShaders;
	static map<string, shared_ptr<PixelShader>> pixelShaders;
	static map<string, shared_ptr<RasterizerState>> rasterizerStates;
	static map<string, shared_ptr<SamplerState>> samplerStates;
	static map<string, shared_ptr<ConstantBuffer>> constantBuffers;

	DirectXObjectPool();
	~DirectXObjectPool();

	public:

	static void SetPolygonMesh(string key, shared_ptr<PolygonMesh> polygonMesh);
	static void SetVertexShader(string key, shared_ptr<VertexShader> vertexShader);
	static void SetPixelShader(string key, shared_ptr<PixelShader> pixelShader);
	static void SetRasterizerState(string key, shared_ptr<RasterizerState> rasterizerState);
	static void SetSamplerState(string key, shared_ptr<SamplerState> samplerState);
	static void SetConstantBuffer(string key, shared_ptr<ConstantBuffer> constantBuffer);

	static shared_ptr<PolygonMesh> GetPolygonMesh(string key);
	static shared_ptr<VertexShader> GetVertexShader(string key);
	static shared_ptr<PixelShader> GetPixelShader(string key);
	static shared_ptr<RasterizerState> GetRasterizerState(string key);
	static shared_ptr<SamplerState> GetSamplerState(string key);
	static shared_ptr<ConstantBuffer> GetConstantBuffer(string key);

	static void Release();
};

