#include "stdafx.h"
#include "TextureQuad.h"


TextureQuad::TextureQuad()
{
	vertexShader = DirectXObjectPool::GetVertexShader("TextureQuad");
	pixelShader = DirectXObjectPool::GetPixelShader("TextureQuadColor");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("Basic");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureQuad");
}


TextureQuad::~TextureQuad()
{

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
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	samplerState->Set(0);
	texture->Set(0);

	polygonMesh->Render();
}
