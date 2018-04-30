#pragma once

#include "DirectXDevice.h"
#include "DirectXObjectPool.h"


enum CameraType
{
	NONE,
	ORTHOGRAPHIC,
	PERSPECTIVE
};


struct CameraCB
{
	XMFLOAT4X4 viewProj;
	XMFLOAT4X4 viewProjInverse;
	XMFLOAT4X4 viewProjCentered;
	XMFLOAT3 cameraPosW;
	float scale;
};


class Camera
{
	protected:

	shared_ptr<ConstantBuffer> constantBuffer;

	XMFLOAT3 position;
	float velocity;

	float zoom;

	float w;
	float h;

	XMFLOAT4X4 view;
	XMFLOAT4X4 viewCentered;
	XMFLOAT4X4 proj;

	virtual void CalculateView() = 0;

	public:

	Camera();
	~Camera();

	virtual void Resize(float width, float height) = 0;

	void SetVelocity(float velocity_);

	const XMMATRIX View();
	const XMMATRIX ViewCentered();
	const XMMATRIX Proj();
	const XMMATRIX ViewProj();
	const XMMATRIX ViewProjCentered();

	virtual XMFLOAT3 ScreenToWorld(float x, float y) = 0;

	virtual void Navigate(float dx, float dy, float dt) = 0;
	virtual void Zoom(float x, float y, float dz) = 0;

	void Set();
};

