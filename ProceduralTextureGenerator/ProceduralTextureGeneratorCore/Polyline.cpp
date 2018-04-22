#include "stdafx.h"
#include "Polyline.h"


struct LineVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT2 uv;
};


PolyLine::PolyLine()
{
	isInitialized = false;
	pointsAreSet = false;

	isSelected = false;
}


PolyLine::~PolyLine()
{

}


HRESULT PolyLine::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("Polyline");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = vertexShader->Init(L"Polyline.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("Polyline", vertexShader);
	}

	linesPixelShader = DirectXObjectPool::GetPixelShader("PolylineLines");
	if(linesPixelShader.get() == nullptr)
	{
		linesPixelShader = make_shared<PixelShader>();

		hr = linesPixelShader->Init(L"Polyline.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("PolylineLines", linesPixelShader);
	}

	pointsPixelShader = DirectXObjectPool::GetPixelShader("PolylinePoints");
	if(pointsPixelShader.get() == nullptr)
	{
		pointsPixelShader = make_shared<PixelShader>();

		hr = pointsPixelShader->Init(L"PolylinePoints.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("PolylinePoints", pointsPixelShader);
	}

	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	if(rasterizerState.get() == nullptr)
	{
		rasterizerState = make_shared<RasterizerState>();

		hr = rasterizerState->Init(D3D11_FILL_SOLID, D3D11_CULL_BACK);
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetRasterizerState("Basic", rasterizerState);
	}

	constantBuffer = DirectXObjectPool::GetConstantBuffer("Polyline");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(PolyLineCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("Polyline", constantBuffer);
	}

	isInitialized = true;

	return hr;
}


HRESULT PolyLine::SetPoints(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType)
{
	HRESULT hr = S_OK;

	pointsAreSet = false;

	points.resize(4);
	points[0] = p0;
	points[1] = XMFLOAT2(p0.x, p0.y - max(16.0f, 0.25f * (p0.y - p1.y)));
	points[2] = XMFLOAT2(p1.x, p1.y + max(16.0f, 0.25f * (p0.y - p1.y)));
	points[3] = p1;

	linesPolygonMesh = make_shared<PolygonMesh>();
	pointsPolygonMesh = make_shared<PolygonMesh>();

	vector<LineVertex> lineVertices(12);
	vector<UINT> lineIndices(18);

	XMFLOAT4 gray = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	for(int i = 0; i < 3; i++)
	{
		XMFLOAT2 a = points[i];
		XMFLOAT2 b = points[i + 1];
		XMFLOAT2 v = XMFLOAT2(b.x - a.x, b.y - a.y);

		int signx = v.y >= 0 ? 1 : -1;
		int signy = v.x >= 0 ? -1 : 1;

		XMFLOAT2 r = XMFLOAT2(width * signx * sqrt(v.y * v.y / (v.x * v.x + v.y * v.y)), width * signy * sqrt(v.x * v.x / (v.x * v.x + v.y * v.y)));

		lineVertices[i * 4 + 0] = { XMFLOAT3(a.x + r.x, a.y + r.y, 0.0f), gray, white, XMFLOAT2(0.0f, 0.0f) };
		lineVertices[i * 4 + 1] = { XMFLOAT3(a.x - r.x, a.y - r.y, 0.0f), gray, white, XMFLOAT2(1.0f, 0.0f) };
		lineVertices[i * 4 + 2] = { XMFLOAT3(b.x + r.x, b.y + r.y, 0.0f), gray, white, XMFLOAT2(0.0f, 1.0f) };
		lineVertices[i * 4 + 3] = { XMFLOAT3(b.x - r.x, b.y - r.y, 0.0f), gray, white, XMFLOAT2(1.0f, 1.0f) };

		lineIndices[i * 6 + 0] = i * 4 + 0;
		lineIndices[i * 6 + 1] = i * 4 + 1;
		lineIndices[i * 6 + 2] = i * 4 + 2;
		lineIndices[i * 6 + 3] = i * 4 + 1;
		lineIndices[i * 6 + 4] = i * 4 + 3;
		lineIndices[i * 6 + 5] = i * 4 + 2;
	}

	hr = linesPolygonMesh->Init((void*) &lineVertices[0], sizeof(LineVertex), lineVertices.size(), &lineIndices[0], lineIndices.size());
	if(FAILED(hr))
	{
		return hr;
	}

	vector<LineVertex> pointVertices(16);
	vector<UINT> pointIndices(24);

	for(int i = 0; i < 4; i++)
	{
		XMFLOAT2 point = points[i];

		pointVertices[i * 4 + 0] = { XMFLOAT3(point.x - width, point.y + width, 0.0f), gray, white, XMFLOAT2(-1.0f, -1.0f) };
		pointVertices[i * 4 + 1] = { XMFLOAT3(point.x + width, point.y + width, 0.0f), gray, white, XMFLOAT2(1.0f, -1.0f) };
		pointVertices[i * 4 + 2] = { XMFLOAT3(point.x - width, point.y - width, 0.0f), gray, white, XMFLOAT2(-1.0f, 1.0f) };
		pointVertices[i * 4 + 3] = { XMFLOAT3(point.x + width, point.y - width, 0.0f), gray, white, XMFLOAT2(1.0f, 1.0f) };

		pointIndices[i * 6 + 0] = i * 4 + 0;
		pointIndices[i * 6 + 1] = i * 4 + 1;
		pointIndices[i * 6 + 2] = i * 4 + 2;
		pointIndices[i * 6 + 3] = i * 4 + 1;
		pointIndices[i * 6 + 4] = i * 4 + 3;
		pointIndices[i * 6 + 5] = i * 4 + 2;
	}

	hr = pointsPolygonMesh->Init((void*) &pointVertices[0], sizeof(LineVertex), pointVertices.size(), &pointIndices[0], pointIndices.size());
	if(FAILED(hr))
	{
		return hr;
	}

	pointsAreSet = true;

	return hr;
}


vector<XMFLOAT2> PolyLine::GetVertices()
{
	return points;
}


void PolyLine::Select()
{
	isSelected = true;
}


void PolyLine::Unselect()
{
	isSelected = false;
}


void PolyLine::RenderLines()
{
	vertexShader->Set();
	linesPixelShader->Set();
	rasterizerState->Set();

	PolyLineCB polyLineCB;
	polyLineCB.state = isSelected ? 1.0f : 0.0f;

	constantBuffer->Update(&polyLineCB);
	constantBuffer->Set(1);

	linesPolygonMesh->Render();
}


void PolyLine::RenderPoints()
{
	vertexShader->Set();
	pointsPixelShader->Set();
	rasterizerState->Set();

	PolyLineCB polyLineCB;
	polyLineCB.state = isSelected ? 1.0f : 0.0f;

	constantBuffer->Update(&polyLineCB);
	constantBuffer->Set(1);

	pointsPolygonMesh->Render();
}


void PolyLine::Render()
{
	if(isInitialized && pointsAreSet)
	{
		RenderLines();
		RenderPoints();
	}
}
