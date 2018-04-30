#pragma once

#include "RenderableObject.h"
#include "DirectXTexture.h"


class TextureQuad : public RenderableObject
{
	private:

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<SamplerState> samplerState;

	DirectXTexturePtr texture;

	public:

	TextureQuad();
	~TextureQuad();

	void SetTexture(shared_ptr<DirectXTexture> texture_);

	void Render();
};

