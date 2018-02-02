#pragma once

#include "RenderableObject.h"
#include "ShaderCompiler.h"
#include "TextureMemory.h"


#define PolylinePtr shared_ptr<PolyLine>


struct PolyLineCB
{
	float state;
	float aligner1;
	float aligner2;
	float aligner3;
};


class PolyLine : public RenderableObject
{
	private:

	bool isInitialized;

	bool isSelected;

	vector<XMFLOAT2> points;
	
	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *linesVertexBuffer;
	ID3D11Buffer *pointsVertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *linesPixelShader;
	ID3D11PixelShader *pointsPixelShader;

	const float width = 1.732051f * 4.0f;

	public:

	PolyLine(shared_ptr<DirectXDevice> device_);
	~PolyLine();

	HRESULT Init(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType);

	vector<XMFLOAT2> GetVertices();

	void Select();
	void Unselect();

	void RenderLines();
	void RenderPoints();
	void Render();

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

