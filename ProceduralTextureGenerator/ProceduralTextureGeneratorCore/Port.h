#pragma once

#include "RenderableObject.h"
#include "TextureMemory.h"


#define PortPtr shared_ptr<Port>


struct PortCB
{
	XMFLOAT4X4 world;
	float ratio;
	float isGrayscale;
	float isMandatoryNotLinked;
	float aligner1;
};


class Port : public RenderableObject
{
	private:

	shared_ptr<PolygonMesh> polygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	bool isGrayscale;
	bool isMandatoryNotLinked;

	float xLocal;
	float yLocal;

	float xGlobal;
	float yGlobal;

	public:

	static const float r0;
	static const float r1;

	Port();
	~Port();

	XMFLOAT2 GetPosition();

	void SetTextureType(TextureType textureType);
	void SetMandatoryNotLinked(bool isMandatoryNotLinked_);
	void SetLocalPosition(float xLocal_, float yLocal_);
	void SetPosition(float xParent, float yParent);

	void Render();
};

