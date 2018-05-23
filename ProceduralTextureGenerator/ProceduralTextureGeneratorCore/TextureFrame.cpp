#include "stdafx.h"
#include "TextureFrame.h"


TextureFrame::TextureFrame()
{
	isSelected = false;

	vertexShader = DirectXObjectPool::GetVertexShader("TextureFrame");
	pixelShader = DirectXObjectPool::GetPixelShader("TextureFrameColor");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	samplerState = DirectXObjectPool::GetSamplerState("LinearClamp");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("TextureFrame");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("TextureFrame");
}


TextureFrame::~TextureFrame()
{

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