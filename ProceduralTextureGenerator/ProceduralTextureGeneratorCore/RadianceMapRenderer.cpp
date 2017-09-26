#include "stdafx.h"
#include "RadianceMapRenderer.h"


RadianceMapRenderer::RadianceMapRenderer()
{

}


RadianceMapRenderer::~RadianceMapRenderer()
{

}


int RadianceMapRenderer::MipLevelsNum(int size)
{
	switch(size)
	{
		case 128:
		{
			return 6;
		}
		case 256:
		{
			return 7;
		}
		case 512:
		{
			return 8;
		}
		case 1024:
		{
			return 9;
		}
		case 2048:
		{
			return 10;
		}
		case 4096:
		{
			return 11;
		}
		default:
		{
			return 2;
		}
	}
}


struct RadianceMapVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};


HRESULT RadianceMapRenderer::Init(shared_ptr<DirectXDevice> device_)
{
	HRESULT hr;

	device = device_;

	ID3DBlob *shaderBlob = 0;

	hr = CompileShaderFromFile(L"RadianceMapShader.fx", "VS", "vs_5_0", &shaderBlob);
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

	hr = CompileShaderFromFile(L"RadianceMapShader.fx", "PS", "ps_5_0", &shaderBlob);
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

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(RadianceMapCB);
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

	hr = device->GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr))
	{
		return hr;
	}


	vector<RadianceMapVertex> radianceMapVertices(6);

	radianceMapVertices[0].pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	radianceMapVertices[0].tex = XMFLOAT2(0.0f, 0.0f);
	radianceMapVertices[1].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	radianceMapVertices[1].tex = XMFLOAT2(1.0f, 0.0f);
	radianceMapVertices[2].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	radianceMapVertices[2].tex = XMFLOAT2(0.0f, 1.0f);

	radianceMapVertices[3].pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	radianceMapVertices[3].tex = XMFLOAT2(1.0f, 0.0f);
	radianceMapVertices[4].pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	radianceMapVertices[4].tex = XMFLOAT2(1.0f, 1.0f);
	radianceMapVertices[5].pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	radianceMapVertices[5].tex = XMFLOAT2(0.0f, 1.0f);

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(RadianceMapVertex) * radianceMapVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &radianceMapVertices[0];

	hr = device->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	return hr;
}


HRESULT RadianceMapRenderer::Render(int size, ID3D11ShaderResourceView *environmentMapSRV, ID3D11Texture2D **radianceMap)
{
	HRESULT hr;

	int mipLevelsNum = MipLevelsNum(size);

	D3D11_TEXTURE2D_DESC radianceMapDesc;
	radianceMapDesc.Usage = D3D11_USAGE_DEFAULT;
	radianceMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	radianceMapDesc.CPUAccessFlags = 0;
	radianceMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	radianceMapDesc.Width = size;
	radianceMapDesc.Height = size;
	radianceMapDesc.MipLevels = mipLevelsNum;
	radianceMapDesc.ArraySize = 6;
	radianceMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	radianceMapDesc.SampleDesc.Count = 1;
	radianceMapDesc.SampleDesc.Quality = 0;

	hr = device->GetDevice()->CreateTexture2D(&radianceMapDesc, 0, radianceMap);
	if(FAILED(hr))
	{
		return hr;
	}


	UINT stride = sizeof(RadianceMapVertex);
	UINT offset = 0;

	device->GetPainter()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device->GetPainter()->IASetInputLayout(inputLayout);
	device->GetPainter()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	device->GetPainter()->RSSetState(rasterizerState);

	device->GetPainter()->VSSetShader(vertexShader, 0, 0);

	device->GetPainter()->PSSetShader(pixelShader, 0, 0);
	device->GetPainter()->PSSetConstantBuffers(0, 1, &constantBuffer);
	device->GetPainter()->PSSetShaderResources(0, 1, &environmentMapSRV);

	for(int faceIndex = 0; faceIndex < 6; faceIndex++)
	{
		int mipSize = size;

		for(int mipLevel = 0; mipLevel < mipLevelsNum; mipLevel++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC mipLevelRTVDesc;
			ZeroMemory(&mipLevelRTVDesc, sizeof(mipLevelRTVDesc));
			mipLevelRTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			mipLevelRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			mipLevelRTVDesc.Texture2DArray.MipSlice = mipLevel;
			mipLevelRTVDesc.Texture2DArray.ArraySize = 1;
			mipLevelRTVDesc.Texture2DArray.FirstArraySlice = faceIndex;

			ID3D11RenderTargetView *mipLevelRTV;
			hr = device->GetDevice()->CreateRenderTargetView(*radianceMap, &mipLevelRTVDesc, &mipLevelRTV);
			if(FAILED(hr))
			{
				return hr;
			}

			const float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			device->GetPainter()->ClearRenderTargetView(mipLevelRTV, clearColor);
			device->GetPainter()->OMSetRenderTargets(1, &mipLevelRTV, nullptr);

			CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float) mipSize, (float) mipSize);
			device->GetPainter()->RSSetViewports(1, &viewport);

			RadianceMapCB radianceMapCB;
			radianceMapCB.roughness = (float) mipLevel / (mipLevelsNum - 1);
			radianceMapCB.faceIndex = faceIndex;

			device->GetPainter()->UpdateSubresource(constantBuffer, 0, 0, &radianceMapCB, 0, 0);

			device->GetPainter()->Draw(6, 0);


			mipLevelRTV->Release();
			mipSize /= 2;
		}
	}

	device->GetPainter()->RSSetState(0);

	return hr;
}


void RadianceMapRenderer::Release()
{
	device = nullptr;

	if(inputLayout) inputLayout->Release();
	if(vertexBuffer) vertexBuffer->Release();
	if(constantBuffer) constantBuffer->Release();
	if(rasterizerState) rasterizerState->Release();
	if(vertexShader) vertexShader->Release();
	if(pixelShader) pixelShader->Release();
}