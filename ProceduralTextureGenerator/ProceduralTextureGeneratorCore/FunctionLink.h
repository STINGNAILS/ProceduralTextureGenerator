#pragma once

#include "RenderableObject.h"
#include "Polyline.h"


#define FunctionLinkPtr shared_ptr<FunctionLink>


class FunctionLink
{
	private:

	bool isSelected;

	int inputNodeIndex;
	int outputNodeIndex;

	int inputPinIndex;

	PolylinePtr polylinePtr;

	public:

	FunctionLink();
	FunctionLink(shared_ptr<DirectXDevice> device_);
	~FunctionLink();

	int GetInputNodeIndex();
	int GetOutputNodeIndex();
	int GetInputPinIndex();
	vector<XMFLOAT2> GetPolylineVertices();

	void SetInputNodeIndex(int inputNodeIndex_);
	void SetOutputNodeIndex(int outputNodeIndex_);
	void SetInputPinIndex(int inputPinIndex_);
	void SetPolylineCoords(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType);

	void Select();
	void Unselect();

	void Render();
};

