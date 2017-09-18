#pragma once


#include <memory>
#include "DirectXDevice.h"
#include "Camera.h"


using namespace std;


class RenderableObject
{
	protected:

	shared_ptr<DirectXDevice> device;

	public:

	RenderableObject();
	~RenderableObject();

	virtual void Render() = 0;

	virtual void Release() = 0;
};
