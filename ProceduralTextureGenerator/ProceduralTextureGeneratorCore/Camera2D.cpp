#include "stdafx.h"
#include "Camera2D.h"


Camera2D::Camera2D()
{
	zoom = 1.0f;
}


Camera2D::~Camera2D()
{

}


void Camera2D::CalculateView()
{
	view(0, 0) = zoom;
	view(1, 0) = 0.0f;
	view(2, 0) = 0.0f;
	view(3, 0) = -position.x;

	view(0, 1) = 0.0f;
	view(1, 1) = zoom;
	view(2, 1) = 0.0f;
	view(3, 1) = -position.y;

	view(0, 2) = 0.0f;
	view(1, 2) = 0.0f;
	view(2, 2) = 1.0f;
	view(3, 2) = -position.z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;
}


void Camera2D::Resize(float width, float height)
{
	w = width;
	h = height;

	XMMATRIX projM;
	projM = XMMatrixOrthographicLH(width, height, 0.0f, 1.0f);
	XMStoreFloat4x4(&proj, projM);
}


void Camera2D::SetPosition(XMFLOAT2 position_)
{
	position = XMFLOAT3(position_.x, position_.y, 0.0f);

	CalculateView();
}

const float Camera2D::Zoom()
{
	return zoom;
}


void Camera2D::Navigate(float dx, float dy, float dt)
{
	position.x -= dx;
	position.y += dy;

	CalculateView();
}


void Camera2D::Zoom(float dz)
{
	zoom *= pow(1.15, dz / 120.0);

	CalculateView();
}