#pragma once

#include "RenderableObject.h"
#include "ShaderCompiler.h"


#define OutputPinPtr shared_ptr<OutputPin>


struct OutputPinCB
{
	XMFLOAT4X4 world;
	float state;
	float ratio;
	float aligner1;
	float aligner2;
};


class OutputPin : public RenderableObject
{
	private:

	bool isInitialized;

	bool isHovered;

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

	const float r0 = 12.0f;
	const float r1 = 1.732051f * 4.0f;

	public:

	OutputPin(shared_ptr<DirectXDevice> device_);
	~OutputPin();

	HRESULT Init();

	XMFLOAT2 GetPosition();

	void SetLocalPosition(float xLocal_, float yLocal_);
	void SetPosition(float xParent, float yParent);

	void OnMouseHover(int xMouse, int yMouse);

	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

