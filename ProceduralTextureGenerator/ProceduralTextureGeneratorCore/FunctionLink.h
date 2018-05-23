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

	int inputPortIndex;

	PolylinePtr polylinePtr;

	public:

	FunctionLink();
	~FunctionLink();

	int GetInputNodeIndex();
	int GetOutputNodeIndex();
	int GetInputPortIndex();
	vector<XMFLOAT2> GetPolylineVertices();

	void SetInputNodeIndex(int inputNodeIndex_);
	void SetOutputNodeIndex(int outputNodeIndex_);
	void SetInputPortIndex(int inputPortIndex_);
	void SetPolylineCoords(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType);

	void Select();
	void Unselect();

	void Render();
};

