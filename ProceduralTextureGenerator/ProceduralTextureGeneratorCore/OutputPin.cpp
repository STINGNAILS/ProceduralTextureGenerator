#include "stdafx.h"
#include "OutputPin.h"


const float OutputPin::r0 = 6.0f;
const float OutputPin::r1 = 1.732051f * 2.0f;


OutputPin::OutputPin()
{
	isHovered = false;
	
	vertexShader = DirectXObjectPool::GetVertexShader("Pin");
	pixelShader = DirectXObjectPool::GetPixelShader("Pin");
	rasterizerState = DirectXObjectPool::GetRasterizerState("Basic");
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Pin");
	polygonMesh = DirectXObjectPool::GetPolygonMesh("Pin");
}


OutputPin::~OutputPin()
{
	
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
	vertexShader->Set();
	pixelShader->Set();
	rasterizerState->Set();

	OutputPinCB outputPinCB;
	XMStoreFloat4x4(&outputPinCB.world, XMMatrixTranspose(XMMatrixTranslation(xGlobal, yGlobal, 0.0f)));
	outputPinCB.state = isHovered ? 1.0f : 0.0f;
	outputPinCB.ratio = r1 / r0;

	constantBuffer->Update(&outputPinCB);
	constantBuffer->Set(1);

	polygonMesh->Render();
}