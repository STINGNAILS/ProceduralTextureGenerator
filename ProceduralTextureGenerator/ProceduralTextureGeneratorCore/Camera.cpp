#include "stdafx.h"
#include "Camera.h"



Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	velocity = 1.0f;

	w = 1.0f;
	h = 1.0f;

	constantBuffer = DirectXObjectPool::GetConstantBuffer("Camera");
}


Camera::~Camera()
{

}


void Camera::SetVelocity(float velocity_)
{
	velocity = velocity_;
}


const XMMATRIX Camera::View()
{
	return XMLoadFloat4x4(&view);
}


const XMMATRIX Camera::ViewCentered()
{
	return XMLoadFloat4x4(&viewCentered);
}


const XMMATRIX Camera::Proj()
{
	return XMLoadFloat4x4(&proj);
}


const XMMATRIX Camera::ViewProj()
{
	return XMMatrixMultiply(View(), Proj());
}


const XMMATRIX Camera::ViewProjCentered()
{
	return XMMatrixMultiply(ViewCentered(), Proj());
}


void Camera::Set()
{
	CameraCB cameraCB;
	XMStoreFloat4x4(&cameraCB.viewProj, XMMatrixTranspose(ViewProj()));
	XMStoreFloat4x4(&cameraCB.viewProjInverse, XMMatrixTranspose(XMMatrixInverse(nullptr, ViewProj())));
	XMStoreFloat4x4(&cameraCB.viewProjCentered, XMMatrixTranspose(ViewProjCentered()));
	cameraCB.cameraPosW = position;
	cameraCB.scale = zoom;

	constantBuffer->Update(&cameraCB);
	constantBuffer->Set(0);
}