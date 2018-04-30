#include "stdafx.h"
#include "FunctionNode.h"


FunctionNode::FunctionNode()
{

}


FunctionNode::FunctionNode(int functionIndex_)
{
	functionIndex = functionIndex_;

	isSelected = false;
	isCalculated = false;

	intParameters = FunctionIntParametersBase(functionIndex);
	floatParameters = FunctionFloatParametersBase(functionIndex);

	textureFramePtr = make_shared<TextureFrame>();
	textureFramePtr->SetLocalPosition(0.0f, 0.0f);

	int functionInputNodesNum = FunctionInputNodesNum(functionIndex);
	inputLinkIndices.resize(functionInputNodesNum);
	inputPinPtrs.resize(functionInputNodesNum);

	for(int i = 0; i < functionInputNodesNum; i++)
	{
		inputLinkIndices[i] = -1;

		inputPinPtrs[i] = make_shared<OutputPin>();
		inputPinPtrs[i]->SetLocalPosition(-31.0f + 62.0f * ((i + 1.0f) / (functionInputNodesNum + 1.0f)), 31.0f);
	}

	if(functionIndex > 4)
	{
		outputPinPtr = make_shared<OutputPin>();
		outputPinPtr->SetLocalPosition(0.0f, -31.0f);
	}
}


FunctionNode::~FunctionNode()
{

}


vector<int> FunctionNode::GetInputLinkIndices()
{
	return inputLinkIndices;
}


vector<int> FunctionNode::GetOutputLinkIndices()
{
	return outputLinkIndices;
}


TextureMemoryPtr FunctionNode::GetTextureMemory()
{
	return textureMemoryPtr;
}


DirectXTexturePtr FunctionNode::GetDirectXTexture()
{
	return directXTexturePtr;
}


vector<int> FunctionNode::GetIntParameters()
{
	return intParameters;
}


vector<float> FunctionNode::GetFloatParameters()
{
	return floatParameters;
}


int FunctionNode::GetFunctionIndex()
{
	return functionIndex;
}


XMFLOAT2 FunctionNode::GetPosition()
{
	return XMFLOAT2(xGlobal, yGlobal);
}


vector<XMFLOAT2> FunctionNode::GetInputPinPositions()
{
	vector<XMFLOAT2> inputPinPositions(inputPinPtrs.size());

	for(int i = 0; i < inputPinPositions.size(); i++)
	{
		inputPinPositions[i] = inputPinPtrs[i]->GetPosition();
	}

	return inputPinPositions;
}


XMFLOAT2 FunctionNode::GetOutputPinPosition()
{
	if(functionIndex > 4)
	{
		return outputPinPtr->GetPosition();
	}
	else
	{
		return XMFLOAT2(100000000.0f, 100000000.0f);
	}
}


void FunctionNode::SetTextureMemory(TextureMemoryPtr textureMemoryPtr_)
{
	textureMemoryPtr = textureMemoryPtr_;

	directXTexturePtr = make_shared<DirectXTexture>(textureMemoryPtr);

	textureFramePtr->SetTexture(directXTexturePtr);
}


void FunctionNode::SetPosition(float xParent, float yParent)
{
	xGlobal = 32.0f * roundf(xParent / 32.0f);
	yGlobal = 32.0f * roundf(yParent / 32.0f);
	
	textureFramePtr->SetPosition(xGlobal, yGlobal);
	for(int i = 0; i < inputPinPtrs.size(); i++)
	{
		inputPinPtrs[i]->SetPosition(xGlobal, yGlobal);
	}

	if(functionIndex > 4)
	{
		outputPinPtr->SetPosition(xGlobal, yGlobal);
	}
}


void FunctionNode::SetIntParameter(int parameterIndex, int value)
{
	if(parameterIndex >= 0 && parameterIndex < intParameters.size())
	{
		intParameters[parameterIndex] = value;
		isCalculated = false;
	}
}


void FunctionNode::SetFloatParameter(int parameterIndex, float value)
{
	if(parameterIndex >= 0 && parameterIndex < floatParameters.size())
	{
		floatParameters[parameterIndex] = value;
		isCalculated = false;
	}
}


bool FunctionNode::IsCalculated()
{
	return isCalculated;
}


void FunctionNode::RequestCalculation()
{
	isCalculated = false;
}


void FunctionNode::MarkAsCalculated()
{
	isCalculated = true;
}


void FunctionNode::AddInputLink(int inputPinIndex, int inputLinkIndex)
{
	inputLinkIndices[inputPinIndex] = inputLinkIndex;
	isCalculated = false;
}


void FunctionNode::AddOutputLink(int outputLinkIndex)
{
	outputLinkIndices.push_back(outputLinkIndex);
}


void FunctionNode::RemoveInputLink(int inputLinkIndex)
{
	for(int i = 0; i < inputLinkIndices.size(); i++)
	{
		if(inputLinkIndices[i] == inputLinkIndex)
		{
			inputLinkIndices[i] = -1;
			isCalculated = false;
		}
	}
}


void FunctionNode::RemoveOutputLink(int outputLinkIndex)
{
	for(int i = 0; i < outputLinkIndices.size(); i++)
	{
		if(outputLinkIndices[i] == outputLinkIndex)
		{
			outputLinkIndices.erase(outputLinkIndices.begin() + i);
		}
	}
}

void FunctionNode::Select()
{
	isSelected = true;

	textureFramePtr->Select();
}


void FunctionNode::Unselect()
{
	isSelected = false;

	textureFramePtr->Unselect();
}


void FunctionNode::Render()
{
	textureFramePtr->Render();
	for(int i = 0; i < inputPinPtrs.size(); i++)
	{
		inputPinPtrs[i]->Render();
	}

	if(functionIndex > 4)
	{
		outputPinPtr->Render();
	}
}
