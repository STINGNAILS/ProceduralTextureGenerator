#include "stdafx.h"
#include "Camera.h"



Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	velocity = 1.0f;

	isInitialized = false;
}


Camera::~Camera()
{

}


HRESULT Camera::Init()
{
	HRESULT hr = S_OK;

	isInitialized = false;
	
	constantBuffer = DirectXObjectPool::GetConstantBuffer("Camera");
	if(constantBuffer.get() == nullptr)
	{
		constantBuffer = make_shared<ConstantBuffer>();

		hr = constantBuffer->Init(sizeof(CameraCB));
		if(FAILED(hr))
		{
			return hr;
		}

		DirectXObjectPool::SetConstantBuffer("Camera", constantBuffer);
	}

	isInitialized = true;

	return hr;
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


const XMFLOAT3 Camera::Position()
{
	return position;
}


const XMFLOAT2 Camera::WidthHeight()
{
	return XMFLOAT2(w, h);
}


void Camera::Set()
{
	if(isInitialized)
	{
		CameraCB cameraCB;
		XMStoreFloat4x4(&cameraCB.viewProj, XMMatrixTranspose(ViewProj()));
		XMStoreFloat4x4(&cameraCB.viewProjCentered, XMMatrixTranspose(ViewProjCentered()));
		cameraCB.cameraPosW = Position();

		constantBuffer->Update(&cameraCB);
		constantBuffer->Set(0);
	}
}