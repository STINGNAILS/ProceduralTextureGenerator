#pragma once

#include "RenderableObject.h"
#include "DirectXTexture.h"


class TextureQuad : public RenderableObject
{
	private:

	bool isInitialized;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;

	DirectXTexturePtr texture;

	public:

	TextureQuad();
	~TextureQuad();

	HRESULT Init();

	void SetTexture(shared_ptr<DirectXTexture> texture_);

	void Render();
};

