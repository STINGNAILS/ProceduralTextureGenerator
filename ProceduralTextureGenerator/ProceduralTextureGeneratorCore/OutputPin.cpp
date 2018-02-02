#include "stdafx.h"
#include "OutputPin.h"


struct OutputPinVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT4 color2;
	XMFLOAT4 color3;
	XMFLOAT2 uv;
};


OutputPin::OutputPin(shared_ptr<DirectXDevice> device_)
{
	device = device_;

	inputLayout = nullptr;
	vertexBuffer = nullptr;
	constantBuffer = nullptr;
	basicRasterizerState = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	isInitialized = false;
}


OutputPin::~OutputPin()
{
	Release();
}


HRESULT OutputPin::Init()
{
	HRESULT hr = S_OK;

	if(!isInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"OutputPin.fx", "VS", "vs_5_0", &shaderBlob);
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
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"OutputPin.fx", "PS", "ps_5_0", &shaderBlob);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = device->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
		shaderBlob->Release();
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

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(OutputPinCB);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &constantBuffer);
		if(FAILED(hr))
		{
			return hr;
		}


		vector<OutputPinVertex> outputPinVertices(21);

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

		outputPinVertices[0].pos = pos1;
		outputPinVertices[0].color0 = red0;
		outputPinVertices[0].color1 = red1;
		outputPinVertices[0].color2 = gray;
		outputPinVertices[0].color3 = white;
		outputPinVertices[0].uv = uv1;

		outputPinVertices[1].pos = pos2;
		outputPinVertices[1].color0 = red0;
		outputPinVertices[1].color1 = red1;
		outputPinVertices[1].color2 = gray;
		outputPinVertices[1].color3 = white;
		outputPinVertices[1].uv = uv2;

		outputPinVertices[2].pos = pos8;
		outputPinVertices[2].color0 = red0;
		outputPinVertices[2].color1 = red1;
		outputPinVertices[2].color2 = gray;
		outputPinVertices[2].color3 = white;
		outputPinVertices[2].uv = uv8;


		outputPinVertices[3].pos = pos1;
		outputPinVertices[3].color0 = red0;
		outputPinVertices[3].color1 = red1;
		outputPinVertices[3].color2 = gray;
		outputPinVertices[3].color3 = white;
		outputPinVertices[3].uv = uv1;

		outputPinVertices[4].pos = pos8;
		outputPinVertices[4].color0 = red0;
		outputPinVertices[4].color1 = red1;
		outputPinVertices[4].color2 = gray;
		outputPinVertices[4].color3 = white;
		outputPinVertices[4].uv = uv8;

		outputPinVertices[5].pos = pos5;
		outputPinVertices[5].color0 = red0;
		outputPinVertices[5].color1 = red1;
		outputPinVertices[5].color2 = gray;
		outputPinVertices[5].color3 = white;
		outputPinVertices[5].uv = uv5;


		outputPinVertices[6].pos = pos2;
		outputPinVertices[6].color0 = red0;
		outputPinVertices[6].color1 = red1;
		outputPinVertices[6].color2 = gray;
		outputPinVertices[6].color3 = white;
		outputPinVertices[6].uv = uv2;

		outputPinVertices[7].pos = pos6;
		outputPinVertices[7].color0 = red0;
		outputPinVertices[7].color1 = red1;
		outputPinVertices[7].color2 = gray;
		outputPinVertices[7].color3 = white;
		outputPinVertices[7].uv = uv6;

		outputPinVertices[8].pos = pos8;
		outputPinVertices[8].color0 = red0;
		outputPinVertices[8].color1 = red1;
		outputPinVertices[8].color2 = gray;
		outputPinVertices[8].color3 = white;
		outputPinVertices[8].uv = uv8;


		outputPinVertices[9].pos = pos3;
		outputPinVertices[9].color0 = green0;
		outputPinVertices[9].color1 = green1;
		outputPinVertices[9].color2 = gray;
		outputPinVertices[9].color3 = white;
		outputPinVertices[9].uv = uv3;

		outputPinVertices[10].pos = pos5;
		outputPinVertices[10].color0 = green0;
		outputPinVertices[10].color1 = green1;
		outputPinVertices[10].color2 = gray;
		outputPinVertices[10].color3 = white;
		outputPinVertices[10].uv = uv5;

		outputPinVertices[11].pos = pos8;
		outputPinVertices[11].color0 = green0;
		outputPinVertices[11].color1 = green1;
		outputPinVertices[11].color2 = gray;
		outputPinVertices[11].color3 = white;
		outputPinVertices[11].uv = uv8;


		outputPinVertices[12].pos = pos3;
		outputPinVertices[12].color0 = green0;
		outputPinVertices[12].color1 = green1;
		outputPinVertices[12].color2 = gray;
		outputPinVertices[12].color3 = white;
		outputPinVertices[12].uv = uv3;

		outputPinVertices[13].pos = pos8;
		outputPinVertices[13].color0 = green0;
		outputPinVertices[13].color1 = green1;
		outputPinVertices[13].color2 = gray;
		outputPinVertices[13].color3 = white;
		outputPinVertices[13].uv = uv8;

		outputPinVertices[14].pos = pos7;
		outputPinVertices[14].color0 = green0;
		outputPinVertices[14].color1 = green1;
		outputPinVertices[14].color2 = gray;
		outputPinVertices[14].color3 = white;
		outputPinVertices[14].uv = uv7;


		outputPinVertices[15].pos = pos4;
		outputPinVertices[15].color0 = blue0;
		outputPinVertices[15].color1 = blue1;
		outputPinVertices[15].color2 = gray;
		outputPinVertices[15].color3 = white;
		outputPinVertices[15].uv = uv4;

		outputPinVertices[16].pos = pos8;
		outputPinVertices[16].color0 = blue0;
		outputPinVertices[16].color1 = blue1;
		outputPinVertices[16].color2 = gray;
		outputPinVertices[16].color3 = white;
		outputPinVertices[16].uv = uv8;

		outputPinVertices[17].pos = pos6;
		outputPinVertices[17].color0 = blue0;
		outputPinVertices[17].color1 = blue1;
		outputPinVertices[17].color2 = gray;
		outputPinVertices[17].color3 = white;
		outputPinVertices[17].uv = uv6;


		outputPinVertices[18].pos = pos4;
		outputPinVertices[18].color0 = blue0;
		outputPinVertices[18].color1 = blue1;
		outputPinVertices[18].color2 = gray;
		outputPinVertices[18].color3 = white;
		outputPinVertices[18].uv = uv4;

		outputPinVertices[19].pos = pos7;
		outputPinVertices[19].color0 = blue0;
		outputPinVertices[19].color1 = blue1;
		outputPinVertices[19].color2 = gray;
		outputPinVertices[19].color3 = white;
		outputPinVertices[19].uv = uv7;

		outputPinVertices[20].pos = pos8;
		outputPinVertices[20].color0 = blue0;
		outputPinVertices[20].color1 = blue1;
		outputPinVertices[20].color2 = gray;
		outputPinVertices[20].color3 = white;
		outputPinVertices[20].uv = uv8;


		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = sizeof(OutputPinVertex) * outputPinVertices.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = &outputPinVertices[0];

		hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		isInitialized = true;
	}

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
		UINT stride = sizeof(OutputPinVertex);
		UINT offset = 0;

		device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetPainter()->IASetInputLayout(inputLayout);
		device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		device->GetPainter()->RSSetState(basicRasterizerState);

		OutputPinCB outputPinCB;
		XMStoreFloat4x4(&outputPinCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
		outputPinCB.state = isHovered ? 1.0f : 0.0f;
		outputPinCB.ratio = r1 / r0;

		device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &outputPinCB, 0, 0);

		device->GetPainter()->VSSetShader(vertexShader, 0, 0);
		device->GetPainter()->VSSetConstantBuffers(1, 1, &constantBuffer);

		device->GetPainter()->PSSetShader(pixelShader, 0, 0);
		device->GetPainter()->PSSetConstantBuffers(1, 1, &constantBuffer);

		device->GetPainter()->Draw(21, 0);

		device->GetPainter()->RSSetState(0);
	}
}


void OutputPin::ReleaseFX()
{
	isInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(constantBuffer) constantBuffer->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}


void OutputPin::ReleaseGeometry()
{
	isInitialized = false;

	if(vertexBuffer) vertexBuffer->Release();
}


void OutputPin::Release()
{
	device = nullptr;

	ReleaseGeometry();
	ReleaseFX();
}