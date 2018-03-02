#pragma once

#include "RenderableObject.h"
#include "TextureMemory.h"
#include "DirectXTexture.h"
#include "FunctionLink.h"
#include "Function.h"
#include "TextureFrame.h"
#include "OutputPin.h"


#define FunctionNodePtr shared_ptr<FunctionNode>


class FunctionNode
{
	private:

	bool isSelected;
	bool isCalculated;

	vector<int> inputLinkIndices;
	vector<int> outputLinkIndices;

	vector<int> intParameters;
	vector<float> floatParameters;

	int functionIndex;

	TextureMemoryPtr textureMemoryPtr;
	DirectXTexturePtr directXTexturePtr;

	float xGlobal;
	float yGlobal;

	TextureFramePtr textureFramePtr;
	vector<OutputPinPtr> inputPinPtrs;
	OutputPinPtr outputPinPtr;

	public:

	FunctionNode();
	FunctionNode(int functionIndex_);
	~FunctionNode();

	vector<int> GetInputLinkIndices();
	vector<int> GetOutputLinkIndices();
	vector<int> GetIntParameters();
	vector<float> GetFloatParameters();
	int GetFunctionIndex();
	TextureMemoryPtr GetTextureMemory();
	DirectXTexturePtr GetDirectXTexture();
	XMFLOAT2 GetPosition();
	vector<XMFLOAT2> GetInputPinPositions();
	XMFLOAT2 GetOutputPinPosition();

	void SetTextureMemory(TextureMemoryPtr textureMemoryPtr_);
	void SetPosition(float xParent, float yParent);
	void SetIntParameter(int parameterIndex, int value);
	void SetFloatParameter(int parameterIndex, float value);

	bool IsCalculated();
	void RequestCalculation();
	void MarkAsCalculated();

	void AddInputLink(int inputPinIndex, int inputLinkIndex);
	void AddOutputLink(int outputLinkIndex);

	void RemoveInputLink(int inputLinkIndex);
	void RemoveOutputLink(int outputLinkIndex);
	
	void Select();
	void Unselect();

	void Render();
};

