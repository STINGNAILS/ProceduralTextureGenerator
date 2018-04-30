#pragma once

#include "RenderableObject.h"
#include "DirectXTexture.h"


class Cube : public RenderableObject
{
	private:

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<SamplerState> samplerState;

	shared_ptr<DirectXTexture> baseColorMap;
	shared_ptr<DirectXTexture> metallicMap;
	shared_ptr<DirectXTexture> roughnessMap;
	shared_ptr<DirectXTexture> normalMap;

	public:

	Cube();
	~Cube();

	void SetBaseColorMap(shared_ptr<DirectXTexture> baseColorMap_);
	void SetMetallicMap(shared_ptr<DirectXTexture> metallicMap_);
	void SetRoughnessMap(shared_ptr<DirectXTexture> roughnessMap_);
	void SetNormalMap(shared_ptr<DirectXTexture> normalMap_);

	void Render();
};

