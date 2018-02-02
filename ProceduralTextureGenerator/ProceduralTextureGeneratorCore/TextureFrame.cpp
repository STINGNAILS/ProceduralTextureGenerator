#include "stdafx.h"
#include "TextureFrame.h"


struct TextureFrameVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};


TextureFrame::TextureFrame(shared_ptr<DirectXDevice> device_)
{
	device = device_;

	inputLayout = nullptr;
	vertexBuffer = nullptr;
	constantBuffer = nullptr;
	basicRasterizerState = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	isInitialized = false;
	isSelected = false;
}


TextureFrame::~TextureFrame()
{
	Release();
}


HRESULT TextureFrame::Init()
{
	HRESULT hr = S_OK;

	if(!isInitialized)
	{
		ID3DBlob *shaderBlob = 0;

		hr = CompileShaderFromFile(L"TextureFrame.fx", "VS", "vs_5_0", &shaderBlob);
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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = device->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
		shaderBlob->Release();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = CompileShaderFromFile(L"TextureFrame.fx", "PS", "ps_5_0", &shaderBlob);
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
		bufferDesc.ByteWidth = sizeof(TextureFrameCB);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &constantBuffer);
		if(FAILED(hr))
		{
			return hr;
		}


		vector<TextureFrameVertex> textureFrameVertices(6);

		XMFLOAT3 pos1 = XMFLOAT3(-64.0f, -64.0f, 0.0f);
		XMFLOAT3 pos2 = XMFLOAT3(64.0f, -64.0f, 0.0f);
		XMFLOAT3 pos3 = XMFLOAT3(-64.0f, 64.0f, 0.0f);
		XMFLOAT3 pos4 = XMFLOAT3(64.0f, 64.0f, 0.0f);

		XMFLOAT2 uv1 = XMFLOAT2(0.0f, 1.0f);
		XMFLOAT2 uv2 = XMFLOAT2(1.0f, 1.0f);
		XMFLOAT2 uv3 = XMFLOAT2(0.0f, 0.0f);
		XMFLOAT2 uv4 = XMFLOAT2(1.0f, 0.0f);

		textureFrameVertices[0].pos = pos1;
		textureFrameVertices[0].uv = uv1;

		textureFrameVertices[1].pos = pos3;
		textureFrameVertices[1].uv = uv3;

		textureFrameVertices[2].pos = pos4;
		textureFrameVertices[2].uv = uv4;


		textureFrameVertices[3].pos = pos1;
		textureFrameVertices[3].uv = uv1;

		textureFrameVertices[4].pos = pos4;
		textureFrameVertices[4].uv = uv4;

		textureFrameVertices[5].pos = pos2;
		textureFrameVertices[5].uv = uv2;


		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = sizeof(TextureFrameVertex) * textureFrameVertices.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = &textureFrameVertices[0];

		hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
		if(FAILED(hr))
		{
			return hr;
		}

		isInitialized = true;
	}

	return hr;
}


void TextureFrame::SetLocalPosition(float xLocal_, float yLocal_)
{
	xLocal = xLocal_;
	yLocal = yLocal_;
}


void TextureFrame::SetPosition(float xParent, float yParent)
{
	xGlobal = xLocal + xParent;
	yGlobal = yLocal + yParent;
}


void TextureFrame::SetTexture(shared_ptr<DirectXTexture> texture_)
{
	texture = texture_;
}

void TextureFrame::Select()
{
	isSelected = true;
}

void TextureFrame::Unselect()
{
	isSelected = false;
}


void TextureFrame::Render()
{
	if(isInitialized)
	{
		UINT stride = sizeof(TextureFrameVertex);
		UINT offset = 0;

		device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetPainter()->IASetInputLayout(inputLayout);
		device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		device->GetPainter()->RSSetState(basicRasterizerState);

		TextureFrameCB textureFrameCB;
		XMStoreFloat4x4(&textureFrameCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
		textureFrameCB.state = isSelected ? 1.0f : 0.0f;

		device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &textureFrameCB, 0, 0);

		device->GetPainter()->VSSetShader(vertexShader, 0, 0);
		device->GetPainter()->VSSetConstantBuffers(1, 1, &constantBuffer);

		device->GetPainter()->PSSetShader(pixelShader, 0, 0);
		device->GetPainter()->PSSetConstantBuffers(1, 1, &constantBuffer);

		texture->Set(0);

		device->GetPainter()->Draw(6, 0);

		device->GetPainter()->RSSetState(0);
	}
}


void TextureFrame::ReleaseFX()
{
	isInitialized = false;

	if(inputLayout) inputLayout->Release();
	if(constantBuffer) constantBuffer->Release();
	if(basicRasterizerState) basicRasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}


void TextureFrame::ReleaseGeometry()
{
	isInitialized = false;

	if(vertexBuffer) vertexBuffer->Release();
}


void TextureFrame::Release()
{
	device = nullptr;

	ReleaseGeometry();
	ReleaseFX();
}
