#pragma once

#include "RenderableObject.h"
#include "TextureMemory.h"
#include "DirectXTexture.h"
#include "FunctionLink.h"
#include "Function.h"
#include "TextureFrame.h"
#include "Port.h"
#include "UtilityFunctions.h"


#define FunctionNodePtr shared_ptr<FunctionNode>


enum CalculatedState
{
	NOT_CALCULATED,
	BEING_CALCULATED,
	CALCULATED
};


class FunctionNode
{
	private:

	CalculatedState calculatedState;

	vector<int> inputLinkIndices;
	vector<int> outputLinkIndices;

	vector<int> intParameters;
	vector<float> floatParameters;

	int functionIndex;

	bool isSelected;

	TextureMemoryPtr textureMemoryPtr;
	DirectXTexturePtr directXTexturePtr;

	float xGlobal;
	float yGlobal;

	TextureFramePtr textureFramePtr;
	vector<PortPtr> inputPortPtrs;
	PortPtr outputPortPtr;

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
	vector<XMFLOAT2> GetInputPortPositions();
	XMFLOAT2 GetOutputPortPosition();

	void SetTextureMemory(TextureMemoryPtr textureMemoryPtr_);
	void SetInputPortTextureType(int inputPortIndex, TextureType textureType);
	void SetPosition(float xParent, float yParent);
	void SetIntParameter(int parameterIndex, int value);
	void SetFloatParameter(int parameterIndex, float value);

	CalculatedState GetCalculatedState();
	void SetCalculatedState(CalculatedState calculatedState_);

	void AddInputLink(int inputPortIndex, int inputLinkIndex);
	void AddOutputLink(int outputLinkIndex);

	void RemoveInputLink(int inputLinkIndex);
	void RemoveOutputLink(int outputLinkIndex);
	
	void Select();
	void Unselect();

	void Render();

	void SaveTextureToFile(LPCWSTR fileName, int formatIndex);
};

