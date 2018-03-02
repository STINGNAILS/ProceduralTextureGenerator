#pragma once


#include "DirectXDevice.h"
#include "DirectXObjectPool.h"


using namespace std;


class RenderableObject
{
	public:

	RenderableObject();
	~RenderableObject();

	virtual void Render() = 0;
};
