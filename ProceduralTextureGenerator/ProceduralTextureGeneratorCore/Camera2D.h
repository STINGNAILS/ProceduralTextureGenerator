#pragma once
#include "Camera.h"


class Camera2D : public Camera
{
	private:

	void CalculateView();

	public:

	Camera2D();
	~Camera2D();

	void Resize(float width, float height);

	void SetPosition(XMFLOAT2 position_);

	XMFLOAT3 ScreenToWorld(float x, float y);
	XMFLOAT3 WorldToScreen(float x, float y);

	void Navigate(float dx, float dy, float dt);
	void Zoom(float x, float y, float dz);
	void Scope(float xMin, float yMin, float xMax, float yMax);
};

