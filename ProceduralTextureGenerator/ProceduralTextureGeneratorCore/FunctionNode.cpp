#include "stdafx.h"
#include "FunctionNode.h"


FunctionNode::FunctionNode()
{

}


FunctionNode::FunctionNode(int functionIndex_)
{
	functionIndex = functionIndex_;

	isSelected = false;
	calculatedState = NOT_CALCULATED;

	intParameters = FunctionIntParametersBase(functionIndex);
	floatParameters = FunctionFloatParametersBase(functionIndex);

	textureFramePtr = make_shared<TextureFrame>();
	textureFramePtr->SetLocalPosition(0.0f, 0.0f);

	vector<InputSlotDescriptor> inputSlotDescriptors = FunctionInputSlotDescriptors(functionIndex);
	int functionInputNodesNum = inputSlotDescriptors.size();
	inputLinkIndices.resize(functionInputNodesNum);
	inputPortPtrs.resize(functionInputNodesNum);

	for(int i = 0; i < functionInputNodesNum; i++)
	{
		inputLinkIndices[i] = -1;

		inputPortPtrs[i] = make_shared<Port>();
		inputPortPtrs[i]->SetTextureType(inputSlotDescriptors[i].textureType);
		inputPortPtrs[i]->SetMandatoryNotLinked(inputSlotDescriptors[i].isMandatory);
		inputPortPtrs[i]->SetLocalPosition(-31.0f + 62.0f * ((i + 1.0f) / (functionInputNodesNum + 1.0f)), 31.0f);
	}

	if(functionIndex > 4)
	{
		TextureType textureType = FunctionOutputSlotTextureType(functionIndex);

		outputPortPtr = make_shared<Port>();
		outputPortPtr->SetTextureType(textureType);
		outputPortPtr->SetMandatoryNotLinked(false);
		outputPortPtr->SetLocalPosition(0.0f, -31.0f);
	}

	directXTexturePtr = make_shared<DirectXTexture>(NotProcessedTexture());
	textureFramePtr->SetTexture(directXTexturePtr);
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


vector<XMFLOAT2> FunctionNode::GetInputPortPositions()
{
	vector<XMFLOAT2> inputPortPositions(inputPortPtrs.size());

	for(int i = 0; i < inputPortPositions.size(); i++)
	{
		inputPortPositions[i] = inputPortPtrs[i]->GetPosition();
	}

	return inputPortPositions;
}


XMFLOAT2 FunctionNode::GetOutputPortPosition()
{
	if(functionIndex > 4)
	{
		return outputPortPtr->GetPosition();
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

	if(functionIndex > 4)
	{
		outputPortPtr->SetTextureType(textureMemoryPtr->GetTextureType());
	}
}


void FunctionNode::SetInputPortTextureType(int inputPortIndex, TextureType textureType)
{
	if(inputPortIndex >= 0 && inputPortIndex < inputPortPtrs.size())
	{
		vector<InputSlotDescriptor> inputSlotDescriptors = FunctionInputSlotDescriptors(functionIndex);

		if(inputSlotDescriptors[inputPortIndex].textureType == COLOR)
		{
			inputPortPtrs[inputPortIndex]->SetTextureType(textureType);
		}
	}
}


void FunctionNode::SetPosition(float xParent, float yParent)
{
	xGlobal = 32.0f * roundf(xParent / 32.0f);
	yGlobal = 32.0f * roundf(yParent / 32.0f);
	
	textureFramePtr->SetPosition(xGlobal, yGlobal);
	for(int i = 0; i < inputPortPtrs.size(); i++)
	{
		inputPortPtrs[i]->SetPosition(xGlobal, yGlobal);
	}

	if(functionIndex > 4)
	{
		outputPortPtr->SetPosition(xGlobal, yGlobal);
	}
}


void FunctionNode::SetIntParameter(int parameterIndex, int value)
{
	if(parameterIndex >= 0 && parameterIndex < intParameters.size())
	{
		intParameters[parameterIndex] = value;
		calculatedState = NOT_CALCULATED;
	}
}


void FunctionNode::SetFloatParameter(int parameterIndex, float value)
{
	if(parameterIndex >= 0 && parameterIndex < floatParameters.size())
	{
		floatParameters[parameterIndex] = value;
		calculatedState = NOT_CALCULATED;
	}
}


CalculatedState FunctionNode::GetCalculatedState()
{
	return calculatedState;
}


void FunctionNode::SetCalculatedState(CalculatedState calculatedState_)
{
	calculatedState = calculatedState_;
}


void FunctionNode::AddInputLink(int inputPortIndex, int inputLinkIndex)
{
	if(inputPortIndex >= 0 && inputPortIndex < inputLinkIndices.size())
	{
		inputLinkIndices[inputPortIndex] = inputLinkIndex;

		inputPortPtrs[inputPortIndex]->SetMandatoryNotLinked(false);

		calculatedState = NOT_CALCULATED;
	}
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

			InputSlotDescriptor inputSlotDescriptor = FunctionInputSlotDescriptors(functionIndex)[i];
			inputPortPtrs[i]->SetTextureType(inputSlotDescriptor.textureType);
			inputPortPtrs[i]->SetMandatoryNotLinked(inputSlotDescriptor.isMandatory);

			calculatedState = NOT_CALCULATED;
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
	for(int i = 0; i < inputPortPtrs.size(); i++)
	{
		inputPortPtrs[i]->Render();
	}

	if(functionIndex > 4)
	{
		outputPortPtr->Render();
	}
}