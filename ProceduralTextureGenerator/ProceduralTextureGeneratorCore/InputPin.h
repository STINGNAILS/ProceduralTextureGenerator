#pragma once

#include "RenderableObject.h"
#include "ShaderCompiler.h"


#define InputPinPtr shared_ptr<InputPin>


struct InputPinCB
{
	XMFLOAT4X4 world;
	float state;
	float aligner1;
	float aligner2;
	float aligner3;
};


class InputPin : public RenderableObject
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

	const float r0 = 16.0f;
	const float r1 = 8.0f;

	public:

	InputPin(shared_ptr<DirectXDevice> device_);
	~InputPin();

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

