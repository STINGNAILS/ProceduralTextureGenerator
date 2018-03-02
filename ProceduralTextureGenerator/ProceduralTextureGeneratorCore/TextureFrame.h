#pragma once

#include "RenderableObject.h"
#include "DirectXTexture.h"
#include "ShaderCompiler.h"


#define TextureFramePtr shared_ptr<TextureFrame>


struct TextureFrameCB
{
	XMFLOAT4X4 world;
	float state;
	int aligner1;
	int aligner2;
	int aligner3;
};


class TextureFrame : public RenderableObject
{
	private:

	bool isInitialized;

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	DirectXTexturePtr texture;

	bool isSelected;

	float xLocal;
	float yLocal;

	float xGlobal;
	float yGlobal;

	public:

	TextureFrame();
	~TextureFrame();

	HRESULT Init();

	void SetLocalPosition(float xLocal_, float yLocal_);
	void SetPosition(float xParent, float yParent);
	void SetTexture(shared_ptr<DirectXTexture> texture_);

	void Select();
	void Unselect();

	void Render();
};

