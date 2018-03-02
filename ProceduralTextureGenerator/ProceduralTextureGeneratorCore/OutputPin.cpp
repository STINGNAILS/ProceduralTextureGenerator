#include "stdafx.h"
#include "OutputPin.h"


struct PinVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT4 color2;
	XMFLOAT4 color3;
	XMFLOAT2 uv;
};


OutputPin::OutputPin()
{
	isInitialized = false;
	isHovered = false;
}


OutputPin::~OutputPin()
{
	
}


HRESULT OutputPin::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("Pin");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = vertexShader->Init(L"OutputPin.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("Pin", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("Pin");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"OutputPin.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("Pin", pixelShader);
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

	constantBuffer = DirectXObjectPool::GetConstantBuffer("Pin");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(OutputPinCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("Pin", constantBuffer);
	}

	polygonMesh = DirectXObjectPool::GetPolygonMesh("Pin");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<PinVertex> pinVertices(13);

		XMFLOAT3 pos1 = XMFLOAT3(-r0, r0, 0.0f);
		XMFLOAT3 pos2 = XMFLOAT3(r0, r0, 0.0f);
		XMFLOAT3 pos3 = XMFLOAT3(-r0, -r0, 0.0f);
		XMFLOAT3 pos4 = XMFLOAT3(r0, -r0, 0.0f);
		XMFLOAT3 pos5 = XMFLOAT3(-r0, 0.577350f * r0, 0.0f);
		XMFLOAT3 pos6 = XMFLOAT3(r0, 0.577350f * r0, 0.0f);
		XMFLOAT3 pos7 = XMFLOAT3(0.0f, -r0, 0.0f);
		XMFLOAT3 pos8 = XMFLOAT3(0.0f, 0.0f, 0.0f);

		XMFLOAT4 red0 = XMFLOAT4(0.75f, 0.0f, 0.0f, 1.0f);
		XMFLOAT4 green0 = XMFLOAT4(0.0f, 0.75f, 0.0f, 1.0f);
		XMFLOAT4 blue0 = XMFLOAT4(0.0f, 0.0f, 0.75f, 1.0f);
		XMFLOAT4 gray = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		XMFLOAT4 red1 = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
		XMFLOAT4 green1 = XMFLOAT4(0.0f, 0.1f, 0.0f, 1.0f);
		XMFLOAT4 blue1 = XMFLOAT4(0.0f, 0.0f, 0.1f, 1.0f);
		XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		XMFLOAT2 uv1 = XMFLOAT2(-1.0f, 1.0f);
		XMFLOAT2 uv2 = XMFLOAT2(1.0f, 1.0f);
		XMFLOAT2 uv3 = XMFLOAT2(-1.0f, -1.0f);
		XMFLOAT2 uv4 = XMFLOAT2(1.0f, -1.0f);
		XMFLOAT2 uv5 = XMFLOAT2(-1.0f, 0.577350f);
		XMFLOAT2 uv6 = XMFLOAT2(1.0f, 0.577350f);
		XMFLOAT2 uv7 = XMFLOAT2(0.0f, -1.0f);
		XMFLOAT2 uv8 = XMFLOAT2(0.0f, 0.0f);

		pinVertices[0] = { pos1, red0, red1, gray, white, uv1 };
		pinVertices[1] = { pos2, red0, red1, gray, white, uv2 };
		pinVertices[2] = { pos5, red0, red1, gray, white, uv5 };
		pinVertices[3] = { pos6, red0, red1, gray, white, uv6 };
		pinVertices[4] = { pos8, red0, red1, gray, white, uv8 };

		pinVertices[5] = { pos3, green0, green1, gray, white, uv3 };
		pinVertices[6] = { pos5, green0, green1, gray, white, uv5 };
		pinVertices[7] = { pos7, green0, green1, gray, white, uv7 };
		pinVertices[8] = { pos8, green0, green1, gray, white, uv8 };

		pinVertices[9] = { pos4, blue0, blue1, gray, white, uv4 };
		pinVertices[10] = { pos6, blue0, blue1, gray, white, uv6 };
		pinVertices[11] = { pos7, blue0, blue1, gray, white, uv7 };
		pinVertices[12] = { pos8, blue0, blue1, gray, white, uv8 };

		vector<UINT> pinIndices(21);

		pinIndices[0] = 0;
		pinIndices[1] = 1;
		pinIndices[2] = 4;

		pinIndices[3] = 0;
		pinIndices[4] = 4;
		pinIndices[5] = 2;

		pinIndices[6] = 1;
		pinIndices[7] = 3;
		pinIndices[8] = 4;

		pinIndices[9] = 5;
		pinIndices[10] = 6;
		pinIndices[11] = 8;

		pinIndices[12] = 5;
		pinIndices[13] = 8;
		pinIndices[14] = 7;

		pinIndices[15] = 9;
		pinIndices[16] = 12;
		pinIndices[17] = 10;

		pinIndices[18] = 9;
		pinIndices[19] = 11;
		pinIndices[20] = 12;

		hr = polygonMesh->Init((void*) &pinVertices[0], sizeof(PinVertex), pinVertices.size(), &pinIndices[0], pinIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("Pin", polygonMesh);
	}

	isInitialized = true;

	return hr;
}


XMFLOAT2 OutputPin::GetPosition()
{
	return XMFLOAT2(xGlobal, yGlobal);
}


void OutputPin::SetLocalPosition(float xLocal_, float yLocal_)
{
	xLocal = xLocal_;
	yLocal = yLocal_;
}


void OutputPin::SetPosition(float xParent, float yParent)
{
	xGlobal = xLocal + xParent;
	yGlobal = yLocal + yParent;
}


void OutputPin::OnMouseHover(int xMouse, int yMouse)
{
	int rsq = (xGlobal - xMouse) * (xGlobal - xMouse) + (yGlobal - yMouse) * (yGlobal - yMouse);
	isHovered = rsq <= r0 * r0;
}


void OutputPin::Render()
{
	if(isInitialized)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		OutputPinCB outputPinCB;
		XMStoreFloat4x4(&outputPinCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
		outputPinCB.state = isHovered ? 1.0f : 0.0f;
		outputPinCB.ratio = r1 / r0;

		constantBuffer->Update(&outputPinCB);
		constantBuffer->Set(1);

		polygonMesh->Render();
	}
}