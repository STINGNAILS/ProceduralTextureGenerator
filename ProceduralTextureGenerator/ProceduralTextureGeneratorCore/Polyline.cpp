#include "stdafx.h"
#include "Polyline.h"


struct LineVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT2 uv;
};


PolyLine::PolyLine(shared_ptr<DirectXDevice> device_)
{
	device = device_;

	inputLayout = nullptr;
	linesVertexBuffer = nullptr;
	pointsVertexBuffer = nullptr;
	constantBuffer = nullptr;
	basicRasterizerState = nullptr;
	vertexShader = nullptr;
	linesPixelShader = nullptr;
	pointsPixelShader = nullptr;

	isInitialized = false;
}


PolyLine::~PolyLine()
{
	Release();
}


HRESULT PolyLine::Init(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType)
{
	HRESULT hr = S_OK;

	if(!isInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"Polyline.fx", "VS", "vs_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
		if(FAILED(hr))
		{
			shaderBlob->Release();
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"Polyline.fx", "PSLines", "ps_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &linesPixelShader);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"Polyline.fx", "PSPoints", "ps_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pointsPixelShader);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(PolyLineCB);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &constantBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;

		hr = device->GetDevice()->CreateRasterizerState(&rasterizerDesc, &basicRasterizerState);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	ReleaseGeometry();

	points.resize(4);
	points[0] = p0;
	points[1] = XMFLOAT2(p0.x, p0.y - max(32.0f, 0.25f * (p0.y - p1.y)));
	points[2] = XMFLOAT2(p1.x, p1.y + max(32.0f, 0.25f * (p0.y - p1.y)));
	points[3] = p1;

	vector<LineVertex> lineVertices(18);

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

		lineVertices[i * 6].pos = XMFLOAT3(a.x + r.x, a.y + r.y, 0.0f);
		lineVertices[i * 6].color0 = gray;
		lineVertices[i * 6].color1 = white;
		lineVertices[i * 6].uv = XMFLOAT2(0.0f, 0.0f);

		lineVertices[i * 6 + 1].pos = XMFLOAT3(a.x - r.x, a.y - r.y, 0.0f);
		lineVertices[i * 6 + 1].color0 = gray;
		lineVertices[i * 6 + 1].color1 = white;
		lineVertices[i * 6 + 1].uv = XMFLOAT2(1.0f, 0.0f);

		lineVertices[i * 6 + 2].pos = XMFLOAT3(b.x + r.x, b.y + r.y, 0.0f);
		lineVertices[i * 6 + 2].color0 = gray;
		lineVertices[i * 6 + 2].color1 = white;
		lineVertices[i * 6 + 2].uv = XMFLOAT2(0.0f, 1.0f);

		lineVertices[i * 6 + 3].pos = XMFLOAT3(a.x - r.x, a.y - r.y, 0.0f);
		lineVertices[i * 6 + 3].color0 = gray;
		lineVertices[i * 6 + 3].color1 = white;
		lineVertices[i * 6 + 3].uv = XMFLOAT2(1.0f, 0.0f);

		lineVertices[i * 6 + 4].pos = XMFLOAT3(b.x - r.x, b.y - r.y, 0.0f);
		lineVertices[i * 6 + 4].color0 = gray;
		lineVertices[i * 6 + 4].color1 = white;
		lineVertices[i * 6 + 4].uv = XMFLOAT2(1.0f, 1.0f);

		lineVertices[i * 6 + 5].pos = XMFLOAT3(b.x + r.x, b.y + r.y, 0.0f);
		lineVertices[i * 6 + 5].color0 = gray;
		lineVertices[i * 6 + 5].color1 = white;
		lineVertices[i * 6 + 5].uv = XMFLOAT2(0.0f, 1.0f);
	}


	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(LineVertex) * lineVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &lineVertices[0];

	hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &linesVertexBuffer);
	if(FAILED(hr))
	{
		return hr;
	}

	vector<LineVertex> pointVertices(24);

	for(int i = 0; i < 4; i++)
	{
		XMFLOAT2 point = points[i];

		pointVertices[i * 6].pos = XMFLOAT3(point.x - width, point.y + width, 0.0f);
		pointVertices[i * 6].color0 = gray;
		pointVertices[i * 6].color1 = white;
		pointVertices[i * 6].uv = XMFLOAT2(-1.0f, -1.0f);

		pointVertices[i * 6 + 1].pos = XMFLOAT3(point.x + width, point.y + width, 0.0f);
		pointVertices[i * 6 + 1].color0 = gray;
		pointVertices[i * 6 + 1].color1 = white;
		pointVertices[i * 6 + 1].uv = XMFLOAT2(1.0f, -1.0f);

		pointVertices[i * 6 + 2].pos = XMFLOAT3(point.x - width, point.y - width, 0.0f);
		pointVertices[i * 6 + 2].color0 = gray;
		pointVertices[i * 6 + 2].color1 = white;
		pointVertices[i * 6 + 2].uv = XMFLOAT2(-1.0f, 1.0f);

		pointVertices[i * 6 + 3].pos = XMFLOAT3(point.x + width, point.y + width, 0.0f);
		pointVertices[i * 6 + 3].color0 = gray;
		pointVertices[i * 6 + 3].color1 = white;
		pointVertices[i * 6 + 3].uv = XMFLOAT2(1.0f, -1.0f);

		pointVertices[i * 6 + 4].pos = XMFLOAT3(point.x + width, point.y - width, 0.0f);
		pointVertices[i * 6 + 4].color0 = gray;
		pointVertices[i * 6 + 4].color1 = white;
		pointVertices[i * 6 + 4].uv = XMFLOAT2(1.0f, 1.0f);

		pointVertices[i * 6 + 5].pos = XMFLOAT3(point.x - width, point.y - width, 0.0f);
		pointVertices[i * 6 + 5].color0 = gray;
		pointVertices[i * 6 + 5].color1 = white;
		pointVertices[i * 6 + 5].uv = XMFLOAT2(-1.0f, 1.0f);
	}

	vbDesc.ByteWidth = sizeof(LineVertex) * pointVertices.size();

	vbData.pSysMem = &pointVertices[0];
	hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &pointsVertexBuffer);
	if(FAILED(hr))
	{
		return hr;
	}

	isInitialized = true;

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
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &linesVertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(basicRasterizerState);

	PolyLineCB polyLineCB;
	polyLineCB.state = isSelected ? 1.0f : 0.0f;

	device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &polyLineCB, 0, 0);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);
	device->GetPainter()->VSSetConstantBuffers(1, 1, &constantBuffer);

	device->GetPainter()->PSSetShader(linesPixelShader, 0, 0);
	device->GetPainter()->PSSetConstantBuffers(1, 1, &constantBuffer);

	device->GetPainter()->Draw(18, 0);

	device->GetPainter()->RSSetState(0);
}

void PolyLine::RenderPoints()
{
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &pointsVertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(basicRasterizerState);

	PolyLineCB polyLineCB;
	polyLineCB.state = isSelected ? 1.0f : 0.0f;

	device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &polyLineCB, 0, 0);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);
	device->GetPainter()->VSSetConstantBuffers(1, 1, &constantBuffer);

	device->GetPainter()->PSSetShader(pointsPixelShader, 0, 0);
	device->GetPainter()->PSSetConstantBuffers(1, 1, &constantBuffer);

	device->GetPainter()->Draw(24, 0);

	device->GetPainter()->RSSetState(0);
}


void PolyLine::Render()
{
	if(isInitialized)
	{
		RenderLines();
		RenderPoints();
	}
}


void PolyLine::ReleaseFX()
{
	isInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(constantBuffer) constantBuffer->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(linesPixelShader) linesPixelShader->Release();
	if(pointsPixelShader) pointsPixelShader->Release();
}


void PolyLine::ReleaseGeometry()
{
	isInitialized = false;

	if(linesVertexBuffer) linesVertexBuffer->Release();
	if(pointsVertexBuffer) pointsVertexBuffer->Release();
}


void PolyLine::Release()
{
	device = nullptr;

	ReleaseGeometry();
	ReleaseFX();
}
