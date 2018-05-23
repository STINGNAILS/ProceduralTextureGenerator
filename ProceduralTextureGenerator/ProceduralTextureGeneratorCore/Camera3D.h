#pragma once

#include "Camera.h"


const XMVECTOR yAxis = {0.0f, 1.0f, 0.0f, 0.0f};


class Camera3D : public Camera
{
	private:

	float yFOV;
	float zNear;
	float zFar;

	float rotationPhi;	//угол относительно xz
	float rotationPsi;	//угол относительно y

	XMFLOAT3 front;
	XMFLOAT3 up;
	XMFLOAT3 right;

	void CalculateTransform();
	void CalculateView();

	public:

	Camera3D();
	~Camera3D();

	void Resize(float width, float height);

	void SetPosition(XMFLOAT3 position_);
	void SetRotation(XMFLOAT2 rotation_);
	void SetYFOV(float yFOV_);
	void SetZNEAR(float zNear_);
	void SetZFAR(float zFar_);

	XMFLOAT3 ScreenToWorld(float x, float y);
	XMFLOAT3 WorldToScreen(float x, float y);

	void Navigate(float dx, float dy, float dt);
	void Zoom(float x, float y, float dz);
	void Scope(float xMin, float yMin, float xMax, float yMax);
};

