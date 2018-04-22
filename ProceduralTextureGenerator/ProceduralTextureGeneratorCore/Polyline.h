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
	bool pointsAreSet;

	shared_ptr<PolygonMesh> linesPolygonMesh;
	shared_ptr<PolygonMesh> pointsPolygonMesh;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> linesPixelShader;
	shared_ptr<PixelShader> pointsPixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<ConstantBuffer> constantBuffer;

	bool isSelected;

	vector<XMFLOAT2> points;

	const float width = 1.732051f * 2.0f;

	void RenderLines();
	void RenderPoints();

	public:

	PolyLine();
	~PolyLine();

	HRESULT Init();
	HRESULT SetPoints(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType);

	vector<XMFLOAT2> GetVertices();

	void Select();
	void Unselect();

	void Render();
};

