#include "stdafx.h"
#include "Camera.h"



Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	velocity = 1.0f;

	cameraConstantBuffer = nullptr;
}


Camera::~Camera()
{
	Release();
}


HRESULT Camera::Init(shared_ptr<DirectXDevice> device_)
{
	HRESULT hr = S_OK;

	device = device_;
	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CameraCB);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	hr = device->GetDevice()->CreateBuffer(&bufferDesc, 0, &cameraConstantBuffer);

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


void Camera::Prepare()
{
	CameraCB cameraCB;
	XMStoreFloat4x4(&cameraCB.viewProj, XMMatrixTranspose(ViewProj()));
	XMStoreFloat4x4(&cameraCB.viewProjCentered, XMMatrixTranspose(ViewProjCentered()));
	cameraCB.cameraPosW = Position();

	device->GetPainter()->UpdateSubresource(cameraConstantBuffer, 0, 0, &cameraCB, 0, 0);

	device->GetPainter()->VSSetConstantBuffers(0, 1, &cameraConstantBuffer);
	device->GetPainter()->PSSetConstantBuffers(0, 1, &cameraConstantBuffer);
}


void Camera::Release()
{
	device = nullptr;

	if(cameraConstantBuffer) cameraConstantBuffer->Release();
}
