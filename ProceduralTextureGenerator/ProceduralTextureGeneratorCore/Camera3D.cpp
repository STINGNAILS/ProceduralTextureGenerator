#include "stdafx.h"
#include "Camera3D.h"



Camera3D::Camera3D()
{
	yFOV = 0.785398f;
	zNear = 0.1f;
	zFar = 1000.0f;

	rotationPhi = 0.0f;
	rotationPsi = 0.0f;
	
	CalculateTransform();
	CalculateView();
}


Camera3D::~Camera3D()
{

}


void Camera3D::CalculateTransform()
{
	float x = cos(rotationPhi) * sin(rotationPsi);
	float y = sin(rotationPhi);
	float z = cos(rotationPhi) * cos(rotationPsi);

	front = XMFLOAT3(x, y, z);

	XMVECTOR frontV = XMVector3Normalize(XMLoadFloat3(&front));
	XMVECTOR rightV = XMVector3Normalize(XMVector3Cross(frontV, yAxis));
	XMVECTOR upV = XMVector3Normalize(XMVector3Cross(rightV, frontV));

	XMStoreFloat3(&up, upV);
	XMStoreFloat3(&right, rightV);
}


void Camera3D::CalculateView()
{
	XMVECTOR positionV = XMLoadFloat3(&position);
	XMVECTOR rightV = XMLoadFloat3(&right);
	XMVECTOR upV = XMLoadFloat3(&up);
	XMVECTOR frontV = XMLoadFloat3(&front);

	float x = -XMVectorGetX(XMVector3Dot(positionV, rightV));
	float y = -XMVectorGetX(XMVector3Dot(positionV, upV));
	float z = -XMVectorGetX(XMVector3Dot(positionV, frontV));

	view(0, 0) = right.x;
	view(1, 0) = right.y;
	view(2, 0) = right.z;
	view(3, 0) = x;

	view(0, 1) = up.x;
	view(1, 1) = up.y;
	view(2, 1) = up.z;
	view(3, 1) = y;

	view(0, 2) = front.x;
	view(1, 2) = front.y;
	view(2, 2) = front.z;
	view(3, 2) = z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;


	viewCentered(0, 0) = right.x;
	viewCentered(1, 0) = right.y;
	viewCentered(2, 0) = right.z;
	viewCentered(3, 0) = 0.0f;

	viewCentered(0, 1) = up.x;
	viewCentered(1, 1) = up.y;
	viewCentered(2, 1) = up.z;
	viewCentered(3, 1) = 0.0f;

	viewCentered(0, 2) = front.x;
	viewCentered(1, 2) = front.y;
	viewCentered(2, 2) = front.z;
	viewCentered(3, 2) = 0.0f;

	viewCentered(0, 3) = 0.0f;
	viewCentered(1, 3) = 0.0f;
	viewCentered(2, 3) = 0.0f;
	viewCentered(3, 3) = 1.0f;
}


void Camera3D::Resize(float width, float height)
{
	w = width;
	h = height;

	XMMATRIX projM;
	projM = XMMatrixPerspectiveFovLH(yFOV, width / height, zNear, zFar);
	XMStoreFloat4x4(&proj, projM);
}


void Camera3D::SetPosition(XMFLOAT3 position_)
{
	position = position_;

	CalculateTransform();
	CalculateView();
}


void Camera3D::SetRotation(XMFLOAT2 rotation_)
{
	rotationPhi = rotation_.x;
	rotationPsi = rotation_.y;

	CalculateTransform();
	CalculateView();
}


void Camera3D::SetYFOV(float yFOV_)
{
	yFOV = yFOV_;
}


void Camera3D::SetZNEAR(float zNear_)
{
	zNear = zNear_;
}


void Camera3D::SetZFAR(float zFar_)
{
	zFar = zFar_;
}


XMFLOAT3 Camera3D::ScreenToWorld(float x, float y)
{
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}


void Camera3D::Navigate(float dx, float dy, float dt)
{
	float dphi = XMConvertToRadians(-0.2f * dy);
	float dpsi = XMConvertToRadians(-0.2f * dx);

	rotationPhi += dphi;
	rotationPsi += dpsi;

	if(rotationPhi > 1.5707f)
	{
		rotationPhi = 1.5707f;
	}
	else if(rotationPhi < -1.5707f)
	{
		rotationPhi = -1.5707f;
	}

	if(GetAsyncKeyState('W') || GetAsyncKeyState(VK_LBUTTON))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR frontV = XMLoadFloat3(&front);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, frontV, positionV));
	}
	if(GetAsyncKeyState('A'))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR rightV = XMLoadFloat3(&right);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, -rightV, positionV));
	}
	if(GetAsyncKeyState('S'))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR frontV = XMLoadFloat3(&front);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, -frontV, positionV));
	}
	if(GetAsyncKeyState('D'))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR rightV = XMLoadFloat3(&right);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, rightV, positionV));
	}
	if(GetAsyncKeyState(VK_SPACE))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, yAxis, positionV));
	}
	if(GetAsyncKeyState(VK_LCONTROL))
	{
		XMVECTOR positionV = XMLoadFloat3(&position);
		XMVECTOR distanceV = XMVectorReplicate(dt * velocity);

		XMStoreFloat3(&position, XMVectorMultiplyAdd(distanceV, -yAxis, positionV));
	}

	CalculateTransform();
	CalculateView();
}


void Camera3D::Zoom(float x, float y, float dz)
{
	//а вот нет зума для такой камеры
}
