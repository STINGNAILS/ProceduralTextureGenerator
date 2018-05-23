#pragma once


#include "DirectXDevice.h"
#include "DirectXObjectPool.h"


class RenderableObject
{
	public:

	RenderableObject();
	~RenderableObject();

	virtual void Render() = 0;
};
