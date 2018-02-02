#pragma once

#include "DirectXDevice.h"


enum CameraType
{
	NONE,
	ORTHOGRAPHIC,
	PERSPECTIVE
};


struct CameraCB
{
	XMFLOAT4X4 viewProj;
	XMFLOAT4X4 viewProjCentered;
	XMFLOAT3 cameraPosW;
	int aligner1;
};


class Camera
{
	protected:

	XMFLOAT3 position;
	float velocity;

	float w;
	float h;

	XMFLOAT4X4 view;
	XMFLOAT4X4 viewCentered;
	XMFLOAT4X4 proj;

	shared_ptr<DirectXDevice> device;

	ID3D11Buffer *cameraConstantBuffer;

	virtual void CalculateView() = 0;

	public:

	Camera();
	~Camera();

	HRESULT Init(shared_ptr<DirectXDevice> device_);

	virtual void Resize(float width, float height) = 0;

	void SetVelocity(float velocity_);

	const XMMATRIX View();
	const XMMATRIX ViewCentered();
	const XMMATRIX Proj();
	const XMMATRIX ViewProj();
	const XMMATRIX ViewProjCentered();

	const XMFLOAT3 Position();
	const XMFLOAT2 WidthHeight();
	virtual const float Zoom() = 0;
	

	virtual void Navigate(float dx, float dy, float dt) = 0;
	virtual void Zoom(float dz) = 0;

	void Prepare();
	
	void Release();
};

