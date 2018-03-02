#pragma once

#include "RenderableObject.h"


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

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	bool isHovered;

	float xLocal;
	float yLocal;

	float xGlobal;
	float yGlobal;

	const float r0 = 12.0f;
	const float r1 = 1.732051f * 4.0f;

	public:

	OutputPin();
	~OutputPin();

	HRESULT Init();

	XMFLOAT2 GetPosition();

	void SetLocalPosition(float xLocal_, float yLocal_);
	void SetPosition(float xParent, float yParent);

	void OnMouseHover(int xMouse, int yMouse);

	void Render();
};

