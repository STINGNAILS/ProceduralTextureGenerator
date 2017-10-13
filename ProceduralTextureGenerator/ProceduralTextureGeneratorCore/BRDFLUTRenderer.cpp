#include "stdafx.h"
#include "BRDFLUTRenderer.h"



BRDFLUTRenderer::BRDFLUTRenderer(shared_ptr<DirectXDevice> device_)
{
	device = device_;
}


BRDFLUTRenderer::~BRDFLUTRenderer()
{
	Release();
}


struct BRDFLUTVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};


HRESULT BRDFLUTRenderer::Init(int size_)
{
	HRESULT hr;

	size = size_;

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(L"BRDFLUTShader.fx", "VS", "vs_5_0", &shaderBlob);
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

	hr = CompileShaderFromFile(L"BRDFLUTShader.fx", "PS", "ps_5_0", &shaderBlob);
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

	hr = device->GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr))
	{
		return hr;
	}


	vector<BRDFLUTVertex> brdfLUTVertexVertices(6);

	brdfLUTVertexVertices[0].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	brdfLUTVertexVertices[0].tex = XMFLOAT2(0.0f, 0.0f);
	brdfLUTVertexVertices[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	brdfLUTVertexVertices[1].tex = XMFLOAT2(1.0f, 0.0f);
	brdfLUTVertexVertices[2].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	brdfLUTVertexVertices[2].tex = XMFLOAT2(0.0f, 1.0f);

	brdfLUTVertexVertices[3].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	brdfLUTVertexVertices[3].tex = XMFLOAT2(1.0f, 0.0f);
	brdfLUTVertexVertices[4].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	brdfLUTVertexVertices[4].tex = XMFLOAT2(1.0f, 1.0f);
	brdfLUTVertexVertices[5].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	brdfLUTVertexVertices[5].tex = XMFLOAT2(0.0f, 1.0f);

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(BRDFLUTVertex) * brdfLUTVertexVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &brdfLUTVertexVertices[0];

	hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	return hr;
}


HRESULT BRDFLUTRenderer::Render(ID3D11Texture2D **brdfLUT)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC brdfLUTDesc;
	brdfLUTDesc.Usage = D3D11_USAGE_DEFAULT;
	brdfLUTDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	brdfLUTDesc.CPUAccessFlags = 0;
	brdfLUTDesc.MiscFlags = 0;
	brdfLUTDesc.Width = size;
	brdfLUTDesc.Height = size;
	brdfLUTDesc.MipLevels = 1;
	brdfLUTDesc.ArraySize = 1;
	brdfLUTDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	brdfLUTDesc.SampleDesc.Count = 1;
	brdfLUTDesc.SampleDesc.Quality = 0;

	hr = device->GetDevice()->CreateTexture2D(&brdfLUTDesc, 0, brdfLUT);
	if(FAILED(hr))
	{
		return hr;
	}


	D3D11_RENDER_TARGET_VIEW_DESC brdfLUTRTVDesc;
	ZeroMemory(&brdfLUTRTVDesc, sizeof(brdfLUTRTVDesc));
	brdfLUTRTVDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	brdfLUTRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	brdfLUTRTVDesc.Texture2DArray.MipSlice = 0;

	ID3D11RenderTargetView *mipLevelRTV;
	hr = device->GetDevice()->CreateRenderTargetView(*brdfLUT, &brdfLUTRTVDesc, &mipLevelRTV);
	if(FAILED(hr))
	{
		return hr;
	}


	UINT stride = sizeof(BRDFLUTVertex);
	UINT offset = 0;

	const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device->GetPainter()->ClearRenderTargetView(mipLevelRTV, clearColor);
	device->GetPainter()->OMSetRenderTargets(1, &mipLevelRTV, nullptr);

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float)size, (float)size);
	device->GetPainter()->RSSetViewports(1, &viewport);

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(rasterizerState);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);

	device->GetPainter()->PSSetShader(pixelShader, 0, 0);

	device->GetPainter()->Draw(6, 0);

	device->GetPainter()->RSSetState(0);

	mipLevelRTV->Release();

	return hr;
}


void BRDFLUTRenderer::Release()
{
	device = nullptr;

	if(inputLayout) inputLayout->Release();
	if(vertexBuffer) vertexBuffer->Release();
	if(rasterizerState) rasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}