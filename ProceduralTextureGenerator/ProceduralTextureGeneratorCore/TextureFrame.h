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
	bool isSelected;

	float xLocal;
	float yLocal;

	float xGlobal;
	float yGlobal;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	DirectXTexturePtr texture;

	public:

	TextureFrame(shared_ptr<DirectXDevice> device_);
	~TextureFrame();

	HRESULT Init();

	void SetLocalPosition(float xLocal_, float yLocal_);
	void SetPosition(float xParent, float yParent);
	void SetTexture(shared_ptr<DirectXTexture> texture_);

	void Select();
	void Unselect();

	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

