#include "stdafx.h"
#include "TextureFrame.h"


struct TextureFrameVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};


TextureFrame::TextureFrame()
{
	isInitialized = false;
	isSelected = false;
}


TextureFrame::~TextureFrame()
{

}


HRESULT TextureFrame::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("TextureFrame");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = vertexShader->Init(L"TextureFrameColor.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("TextureFrame", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("TextureFrameGrayscale");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"TextureFrameGrayscale.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("TextureFrameGrayscale", pixelShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("TextureFrameColor");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"TextureFrameColor.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("TextureFrameColor", pixelShader);
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

	samplerState = DirectXObjectPool::GetSamplerState("Basic");
	if(rasterizerState.get() == nullptr)
	{
		samplerState = make_shared<SamplerState>();

		hr = samplerState->Init(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0);
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetSamplerState("Basic", samplerState);
	}

	constantBuffer = DirectXObjectPool::GetConstantBuffer("TextureFrame");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(TextureFrameCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("TextureFrame", constantBuffer);
	}
	
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureFrame");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<TextureFrameVertex> textureFrameVertices(4);

		textureFrameVertices[0] = { XMFLOAT3(-31.0f, -31.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
		textureFrameVertices[1] = { XMFLOAT3(31.0f, -31.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
		textureFrameVertices[2] = { XMFLOAT3(-31.0f, 31.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
		textureFrameVertices[3] = { XMFLOAT3(31.0f, 31.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

		vector<UINT> textureFrameIndices(6);

		textureFrameIndices[0] = 0;
		textureFrameIndices[1] = 2;
		textureFrameIndices[2] = 3;
		textureFrameIndices[3] = 0;
		textureFrameIndices[4] = 3;
		textureFrameIndices[5] = 1;

		hr = polygonMesh->Init((void*) &textureFrameVertices[0], sizeof(TextureFrameVertex), textureFrameVertices.size(), &textureFrameIndices[0], textureFrameIndices.size());
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPolygonMesh("TextureFrame", polygonMesh);
	}

	isInitialized = true;

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

	if(texture.get() != nullptr)
	{
		switch(texture->GetTextureType())
		{
			case GRAYSCALE:
			{
				pixelShader = DirectXObjectPool::GetPixelShader("TextureFrameGrayscale");
				break;
			}
			case COLOR:
			{
				pixelShader = DirectXObjectPool::GetPixelShader("TextureFrameColor");
				break;
			}
		}
	}
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
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		TextureFrameCB textureFrameCB;
		XMStoreFloat4x4(&textureFrameCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
		textureFrameCB.state = isSelected ? 1.0f : 0.0f;

		constantBuffer->Update(&textureFrameCB);
		constantBuffer->Set(1);

		samplerState->Set(0);
		texture->Set(0);

		polygonMesh->Render();
	}
}