#pragma once
#include "Camera.h"


class Camera2D : public Camera
{
	private:

	float zoom;

	void CalculateView();

	public:

	Camera2D();
	~Camera2D();

	void Resize(float width, float height);

	void SetPosition(XMFLOAT2 position_);

	const float Zoom();

	void Navigate(float dx, float dy, float dt);
	void Zoom(float dz);
};

