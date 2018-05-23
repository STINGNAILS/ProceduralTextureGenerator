#include "stdafx.h"
#include "Port.h"


const float Port::r0 = 6.0f;
const float Port::r1 = 1.732051f * 2.0f;


Port::Port()
{
	isGrayscale = false;
	isMandatoryNotLinked = true;
	
	vertexShader = DirectXObjectPool::GetVertexShader("Port");
	pixelShader = DirectXObjectPool::GetPixelShader("Port");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Port");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Port");
}


Port::~Port()
{
	
}


XMFLOAT2 Port::GetPosition()
{
	return XMFLOAT2(xGlobal, yGlobal);
}


void Port::SetTextureType(TextureType textureType)
{
	switch(textureType)
	{
		case COLOR:
		{
			isGrayscale = false;
			break;
		}
		case GRAYSCALE:
		{
			isGrayscale = true;
			break;
		}
	}
}


void Port::SetMandatoryNotLinked(bool isMandatoryNotLinked_)
{
	isMandatoryNotLinked = isMandatoryNotLinked_;
}


void Port::SetLocalPosition(float xLocal_, float yLocal_)
{
	xLocal = xLocal_;
	yLocal = yLocal_;
}


void Port::SetPosition(float xParent, float yParent)
{
	xGlobal = xLocal + xParent;
	yGlobal = yLocal + yParent;
}


void Port::Render()
{
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	PortCB portCB;
	XMStoreFloat4x4(&portCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
	portCB.ratio = r1 / r0;
	portCB.isGrayscale = isGrayscale ? 1.0f : 0.0f;
	portCB.isMandatoryNotLinked = isMandatoryNotLinked ? 1.0f : 0.0f;

	constantBuffer->Update(&portCB);
	constantBuffer->Set(1);

	polygonMesh->Render();
}