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


XMFLOAT3 Camera2D::ScreenToWorld(float x, float y)
{
	XMFLOAT3 world;
	world.x = (position.x + w * (x - 0.5f)) / zoom;
	world.y = (position.y + h * (y - 0.5f)) / zoom;
	world.z = 0.0f;
	
	return world;
}


XMFLOAT3 Camera2D::WorldToScreen(float x, float y)
{
	XMFLOAT3 screen;
	screen.x = (zoom * x - position.x) / w + 0.5f;
	screen.y = (zoom * y - position.y) / h + 0.5f;
	screen.z = 0.0f;

	return screen;
}


void Camera2D::Navigate(float dx, float dy, float dt)
{
	position.x -= dx;
	position.y += dy;

	CalculateView();
}


void Camera2D::Zoom(float x, float y, float dz)
{
	XMFLOAT3 world = ScreenToWorld(x, y);
	float s1 = zoom;
	float s2 = zoom * pow(1.15, dz / 120.0);

	zoom = s2;
	position.x += (s2 - s1) * world.x;
	position.y += (s2 - s1) * world.y;

	CalculateView();
}


void Camera2D::Scope(float xMin, float yMin, float xMax, float yMax)
{
	float sX = w / (xMax - xMin);
	float sY = h / (yMax - yMin);
	zoom = min(sX, sY);

	position.x = zoom * (xMin + xMax) / 2.0f;
	position.y = zoom * (yMin + yMax) / 2.0f;

	CalculateView();
}