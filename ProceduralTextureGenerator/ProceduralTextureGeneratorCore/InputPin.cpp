#include "stdafx.h"
#include "InputPin.h"


struct InputPinVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
};


InputPin::InputPin(shared_ptr<DirectXDevice> device_)
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


InputPin::~InputPin()
{
	Release();
}


HRESULT InputPin::Init()
{
	HRESULT hr = S_OK;

	if(!isInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"InputPin.fx", "VS", "vs_5_0", &shaderBlob);
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
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"InputPin.fx", "PS", "ps_5_0", &shaderBlob);
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
		bufferDesc.ByteWidth = sizeof(InputPinCB);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &constantBuffer);
		if(FAILED(hr))
		{
			return hr;
		}
		

		vector<InputPinVertex> inputPinVertices(21);

		XMFLOAT3 pos01 = XMFLOAT3(-0.866025f * r0, 0.5f * r0, 0.0f);
		XMFLOAT3 pos02 = XMFLOAT3(0.866025f * r0, 0.5f * r0, 0.0f);
		XMFLOAT3 pos03 = XMFLOAT3(0.0f, -r0, 0.0f);
		XMFLOAT3 pos11 = XMFLOAT3(-0.866025f * r1, 0.5f * r1, 0.0f);
		XMFLOAT3 pos12 = XMFLOAT3(0.866025f * r1, 0.5f * r1, 0.0f);
		XMFLOAT3 pos13 = XMFLOAT3(0.0f, -r1, 0.0f);

		XMFLOAT4 red0 = XMFLOAT4(0.75f, 0.0f, 0.0f, 1.0f);
		XMFLOAT4 green0 = XMFLOAT4(0.0f, 0.75f, 0.0f, 1.0f);
		XMFLOAT4 blue0 = XMFLOAT4(0.0f, 0.0f, 0.75f, 1.0f);
		XMFLOAT4 gray = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		XMFLOAT4 red1 = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);
		XMFLOAT4 green1 = XMFLOAT4(0.0f, 0.4f, 0.0f, 1.0f);
		XMFLOAT4 blue1 = XMFLOAT4(0.0f, 0.0f, 0.4f, 1.0f);
		XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		inputPinVertices[0].pos = pos01;
		inputPinVertices[0].color0 = red0;
		inputPinVertices[0].color1 = red0;

		inputPinVertices[1].pos = pos02;
		inputPinVertices[1].color0 = red0;
		inputPinVertices[1].color1 = red0;

		inputPinVertices[2].pos = pos11;
		inputPinVertices[2].color0 = red1;
		inputPinVertices[2].color1 = red0;


		inputPinVertices[3].pos = pos02;
		inputPinVertices[3].color0 = red0;
		inputPinVertices[3].color1 = red0;

		inputPinVertices[4].pos = pos12;
		inputPinVertices[4].color0 = red1;
		inputPinVertices[4].color1 = red0;

		inputPinVertices[5].pos = pos11;
		inputPinVertices[5].color0 = red1;
		inputPinVertices[5].color1 = red0;


		inputPinVertices[6].pos = pos03;
		inputPinVertices[6].color0 = green0;
		inputPinVertices[6].color1 = green0;

		inputPinVertices[7].pos = pos01;
		inputPinVertices[7].color0 = green0;
		inputPinVertices[7].color1 = green0;

		inputPinVertices[8].pos = pos13;
		inputPinVertices[8].color0 = green1;
		inputPinVertices[8].color1 = green0;


		inputPinVertices[9].pos = pos01;
		inputPinVertices[9].color0 = green0;
		inputPinVertices[9].color1 = green0;

		inputPinVertices[10].pos = pos11;
		inputPinVertices[10].color0 = green1;
		inputPinVertices[10].color1 = green0;

		inputPinVertices[11].pos = pos13;
		inputPinVertices[11].color0 = green1;
		inputPinVertices[11].color1 = green0;


		inputPinVertices[12].pos = pos02;
		inputPinVertices[12].color0 = blue0;
		inputPinVertices[12].color1 = blue0;

		inputPinVertices[13].pos = pos03;
		inputPinVertices[13].color0 = blue0;
		inputPinVertices[13].color1 = blue0;

		inputPinVertices[14].pos = pos12;
		inputPinVertices[14].color0 = blue1;
		inputPinVertices[14].color1 = blue0;


		inputPinVertices[15].pos = pos03;
		inputPinVertices[15].color0 = blue0;
		inputPinVertices[15].color1 = blue0;

		inputPinVertices[16].pos = pos13;
		inputPinVertices[16].color0 = blue1;
		inputPinVertices[16].color1 = blue0;

		inputPinVertices[17].pos = pos12;
		inputPinVertices[17].color0 = blue1;
		inputPinVertices[17].color1 = blue0;


		inputPinVertices[18].pos = pos11;
		inputPinVertices[18].color0 = gray;
		inputPinVertices[18].color1 = white;

		inputPinVertices[19].pos = pos12;
		inputPinVertices[19].color0 = gray;
		inputPinVertices[19].color1 = white;

		inputPinVertices[20].pos = pos13;
		inputPinVertices[20].color0 = gray;
		inputPinVertices[20].color1 = white;


		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = sizeof(InputPinVertex) * inputPinVertices.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = &inputPinVertices[0];

		hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		isInitialized = true;
	}

	return hr;
}


XMFLOAT2 InputPin::GetPosition()
{
	return XMFLOAT2(xGlobal, yGlobal);
}


void InputPin::SetLocalPosition(float xLocal_, float yLocal_)
{
	xLocal = xLocal_;
	yLocal = yLocal_;
}


void InputPin::SetPosition(float xParent, float yParent)
{
	xGlobal = xLocal + xParent;
	yGlobal = yLocal + yParent;
}


void InputPin::OnMouseHover(int xMouse, int yMouse)
{
	int rsq = (xGlobal - xMouse) * (xGlobal - xMouse) + (yGlobal - yMouse) * (yGlobal - yMouse);
	isHovered = rsq <= r0 * r0;
}


void InputPin::Render()
{
	if(isInitialized)
	{
		UINT stride = sizeof(InputPinVertex);
		UINT offset = 0;

		device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetPainter()->IASetInputLayout(inputLayout);
		device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		device->GetPainter()->RSSetState(basicRasterizerState);

		InputPinCB inputPinCB;
		XMStoreFloat4x4(&inputPinCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
		inputPinCB.state = isHovered ? 1.0f : 0.0f;

		device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &inputPinCB, 0, 0);

		device->GetPainter()->VSSetShader(vertexShader, 0, 0);
		device->GetPainter()->VSSetConstantBuffers(1, 1, &constantBuffer);

		device->GetPainter()->PSSetShader(pixelShader, 0, 0);
		device->GetPainter()->PSSetConstantBuffers(1, 1, &constantBuffer);

		device->GetPainter()->Draw(21, 0);

		device->GetPainter()->RSSetState(0);
	}
}


void InputPin::ReleaseFX()
{
	isInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(constantBuffer) constantBuffer->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}


void InputPin::ReleaseGeometry()
{
	isInitialized = false;

	if(vertexBuffer) vertexBuffer->Release();
}


void InputPin::Release()
{
	device = nullptr;

	ReleaseGeometry();
	ReleaseFX();
}
