#pragma once

#include "RenderableObject.h"
#include "TextureMemory.h"
#include "DirectXTexture.h"
#include "UtilityFunctions.h"


class BackgroundGrid : public RenderableObject
{
	private:

	bool isInitialized;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<SamplerState> samplerState;

	DirectXTexturePtr texture;

	public:

	BackgroundGrid();
	~BackgroundGrid();

	HRESULT Init();

	void Render();
};

