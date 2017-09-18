#pragma once

#include "Camera.h"


class Camera3D : public Camera
{
	private:

	float yFOV;
	float zNear;
	float zFar;

	float rotationPhi;	//���� ������������ xz
	float rotationPsi;	//���� ������������ y

	XMFLOAT3 front;
	XMFLOAT3 up;
	XMFLOAT3 right;

	XMVECTOR yAxis;

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

	void Navigate(float dx, float dy, float dt);
	void Zoom(float dz);
};

