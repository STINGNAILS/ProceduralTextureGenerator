#include "stdafx.h"
#include "TextureQuad.h"


struct TextureFrameVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};


TextureQuad::TextureQuad()
{
	isInitialized = false;
}


TextureQuad::~TextureQuad()
{

}


HRESULT TextureQuad::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;

	vertexShader = DirectXObjectPool::GetVertexShader("TextureQuad");
	if(vertexShader.get() == nullptr)
	{
		vertexShader = make_shared<VertexShader>();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = vertexShader->Init(L"TextureQuadColor.fx", layout, ARRAYSIZE(layout));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetVertexShader("TextureQuad", vertexShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("TextureQuadGrayscale");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"TextureQuadGrayscale.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("TextureQuadGrayscale", pixelShader);
	}

	pixelShader = DirectXObjectPool::GetPixelShader("TextureQuadColor");
	if(pixelShader.get() == nullptr)
	{
		pixelShader = make_shared<PixelShader>();

		hr = pixelShader->Init(L"TextureQuadColor.fx");
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetPixelShader("TextureQuadColor", pixelShader);
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

	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureQuad");
	if(polygonMesh.get() == nullptr)
	{
		polygonMesh = make_shared<PolygonMesh>();

		vector<TextureFrameVertex> textureFrameVertices(4);

		textureFrameVertices[0] = { XMFLOAT3(-256.0f, -256.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
		textureFrameVertices[1] = { XMFLOAT3(256.0f, -256.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
		textureFrameVertices[2] = { XMFLOAT3(-256.0f, 256.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
		textureFrameVertices[3] = { XMFLOAT3(256.0f, 256.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

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

		DirectXObjectPool::SetPolygonMesh("TextureQuad", polygonMesh);
	}

	isInitialized = true;

	return hr;
}


void TextureQuad::SetTexture(shared_ptr<DirectXTexture> texture_)
{
	texture = texture_;

	if(texture.get() != nullptr)
	{
		switch(texture->GetTextureType())
		{
			case GRAYSCALE:
			{
				pixelShader = DirectXObjectPool::GetPixelShader("TextureQuadGrayscale");
				break;
			}
			case COLOR:
			{
				pixelShader = DirectXObjectPool::GetPixelShader("TextureQuadColor");
				break;
			}
		}
	}
}


void TextureQuad::Render()
{
	if(isInitialized)
	{
		vertexShader->Set();
		pixelShader->Set();
		rasterizerState->Set();

		texture->Set(0);

		polygonMesh->Render();
	}
}
