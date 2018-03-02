#include "stdafx.h"
#include "FunctionLink.h"


FunctionLink::FunctionLink()
{
	inputNodeIndex = -1;
	outputNodeIndex = -1;
	inputPinIndex = -1;

	polylinePtr = make_shared<PolyLine>();
	polylinePtr->Init();

	isSelected = false;
}


FunctionLink::~FunctionLink()
{

}


int FunctionLink::GetInputNodeIndex()
{
	return inputNodeIndex;
}


int FunctionLink::GetOutputNodeIndex()
{
	return outputNodeIndex;
}


int FunctionLink::GetInputPinIndex()
{
	return inputPinIndex;
}


vector<XMFLOAT2> FunctionLink::GetPolylineVertices()
{
	return polylinePtr->GetVertices();
}


void FunctionLink::SetInputNodeIndex(int inputNodeIndex_)
{
	inputNodeIndex = inputNodeIndex_;
}


void FunctionLink::SetOutputNodeIndex(int outputNodeIndex_)
{
	outputNodeIndex = outputNodeIndex_;
}


void FunctionLink::SetInputPinIndex(int inputPinIndex_)
{
	inputPinIndex = inputPinIndex_;
}


void FunctionLink::SetPolylineCoords(XMFLOAT2 p0, XMFLOAT2 p1, TextureType textureType)
{
	polylinePtr->SetPoints(p0, p1, textureType);
}


void FunctionLink::Select()
{
	isSelected = true;

	polylinePtr->Select();
}


void FunctionLink::Unselect()
{
	isSelected = false;

	polylinePtr->Unselect();
}


void FunctionLink::Render()
{
	polylinePtr->Render();
}
