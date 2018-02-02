#include "stdafx.h"
#include "FunctionGraph.h"


FunctionGraph::FunctionGraph(shared_ptr<DirectXDevice> device_)
{
	device = device_;
	newFunctionLink = make_shared<FunctionLink>(device);

	nextNodeIndex = 0;
	nextLinkIndex = 0;

	AddNode(0, -384.0f, 0.0f);
	AddNode(1, -128.0f, 0.0f);
	AddNode(1, 128.0f, 0.0f);
	AddNode(0, 384.0f, 0.0f);

	selectedNodeIndex = -1;
	selectedLinkIndex = -1;

	interactionState = INTERACTION_NONE;

	newFunctionLink = nullptr;

	Process();
}


FunctionGraph::~FunctionGraph()
{
	Release();
}


DirectXTexturePtr FunctionGraph::GetBaseColorTexture()
{
	return functionNodes[0].GetDirectXTexture();
}


DirectXTexturePtr FunctionGraph::GetMetallicTexture()
{
	return functionNodes[1].GetDirectXTexture();
}


DirectXTexturePtr FunctionGraph::GetRoughnessTexture()
{
	return functionNodes[2].GetDirectXTexture();
}


DirectXTexturePtr FunctionGraph::GetNormalTexture()
{
	return functionNodes[3].GetDirectXTexture();
}


int FunctionGraph::GetSelectedNodeIndex()
{
	return selectedNodeIndex;
}


int FunctionGraph::GetSelectedNodeFunctionIndex()
{
	if(selectedNodeIndex != -1)
	{
		return functionNodes[selectedNodeIndex].GetFunctionIndex();
	}
	else
	{
		return -1;
	}
}


int FunctionGraph::GetSelectedNodeIntParameter(int parameterIndex)
{
	if(selectedNodeIndex != -1)
	{
		vector<int> intParameters = functionNodes[selectedNodeIndex].GetIntParameters();
		if(parameterIndex >= 0 && parameterIndex < intParameters.size())
		{
			return intParameters[parameterIndex];
		}
	}

	return 0;
}


float FunctionGraph::GetSelectedNodeFloatParameter(int parameterIndex)
{
	if(selectedNodeIndex != -1)
	{
		vector<float> floatParameters = functionNodes[selectedNodeIndex].GetFloatParameters();
		if(parameterIndex >= 0 && parameterIndex < floatParameters.size())
		{
			return floatParameters[parameterIndex];
		}
	}
	
	return 0.0f;
}


void FunctionGraph::SetSelectedNodeIntParameter(int parameterIndex, int value)
{
	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].SetIntParameter(parameterIndex, value);
		Process();
	}
}


void FunctionGraph::SetSelectedNodeFloatParameter(int parameterIndex, float value)
{
	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].SetFloatParameter(parameterIndex, value);
		Process();
	}
}


enum NodeColor
{
	WHITE,
	GRAY,
	BLACK
};


void FunctionGraph::Validate(map<int, FunctionNode> &functionNodesCopy, map<int, FunctionLink> &functionLinksCopy)
{
	vector<int> graphTraverseOrderCopy;

	stack<int> traverseStack;

	map<int, NodeColor> nodeColors;
	for(auto it = functionNodesCopy.begin(); it != functionNodesCopy.end(); it++)
	{
		nodeColors[it->first] = WHITE;
	}

	bool isDirectedAcyclic = true;

	for(auto it = functionNodesCopy.begin(); it != functionNodesCopy.end(); it++)
	{
		int currentNodeIndex = it->first;

		if(nodeColors[currentNodeIndex] == WHITE)
		{
			traverseStack.push(currentNodeIndex);
			nodeColors[currentNodeIndex] = GRAY;

			while(!traverseStack.empty() && isDirectedAcyclic)
			{
				currentNodeIndex = traverseStack.top();
				FunctionNode currentNode = functionNodesCopy[currentNodeIndex];
				vector<int> inputLinkIndices = currentNode.GetInputLinkIndices();

				vector<int> inputNodeIndices;
				for(int i = 0; i < inputLinkIndices.size(); i++)
				{
					if(inputLinkIndices[i] != -1)
					{
						inputNodeIndices.push_back(functionLinksCopy[inputLinkIndices[i]].GetInputNodeIndex());
					}
				}

				bool inputsAreBlack = true;

				for(int j = inputNodeIndices.size() - 1; j >= 0; j--)
				{
					if(nodeColors[inputNodeIndices[j]] == WHITE)
					{
						traverseStack.push(inputNodeIndices[j]);
						nodeColors[inputNodeIndices[j]] = GRAY;
						inputsAreBlack = false;
					}
					else if(nodeColors[inputNodeIndices[j]] == GRAY)
					{
						isDirectedAcyclic = false;
						break;
					}
				}

				if(inputsAreBlack)
				{
					traverseStack.pop();
					graphTraverseOrderCopy.push_back(currentNodeIndex);
					nodeColors[currentNodeIndex] = BLACK;
				}
			}
		}
	}

	if(isDirectedAcyclic)
	{
		functionNodes = functionNodesCopy;
		functionLinks = functionLinksCopy;
		graphTraverseOrder = graphTraverseOrderCopy;

		Process();
	}
}


void FunctionGraph::AddNode(int functionIndex, float x, float y)
{
	FunctionNode newFunctionNode = FunctionNode(device, functionIndex);
	newFunctionNode.SetPosition(x, y);

	functionNodes[nextNodeIndex] = newFunctionNode;
	graphTraverseOrder.push_back(nextNodeIndex);

	nextNodeIndex++;
}


void FunctionGraph::AddLink(int inputNodeIndex, int outputNodeIndex, int inputPinIndex)
{
	map<int, FunctionNode> functionNodesCopy = functionNodes;
	map<int, FunctionLink> functionLinksCopy = functionLinks;

	int linkToRemoveIndex = functionNodesCopy[outputNodeIndex].GetInputLinkIndices()[inputPinIndex];
	if(linkToRemoveIndex != -1)
	{
		functionNodesCopy[functionLinksCopy[linkToRemoveIndex].GetInputNodeIndex()].RemoveOutputLink(linkToRemoveIndex);
		functionNodesCopy[functionLinksCopy[linkToRemoveIndex].GetOutputNodeIndex()].RemoveOutputLink(linkToRemoveIndex);

		functionLinksCopy.erase(linkToRemoveIndex);
	}

	functionLinksCopy[nextLinkIndex] = FunctionLink(device);
	functionLinksCopy[nextLinkIndex].SetInputNodeIndex(inputNodeIndex);
	functionLinksCopy[nextLinkIndex].SetOutputNodeIndex(outputNodeIndex);
	functionLinksCopy[nextLinkIndex].SetInputPinIndex(inputPinIndex);
	functionLinksCopy[nextLinkIndex].SetPolylineCoords(functionNodes[inputNodeIndex].GetOutputPinPosition(), functionNodes[outputNodeIndex].GetInputPinPositions()[inputPinIndex], COLOR);

	functionNodesCopy[inputNodeIndex].AddOutputLink(nextLinkIndex);
	functionNodesCopy[outputNodeIndex].AddInputLink(inputPinIndex, nextLinkIndex);

	nextLinkIndex++;

	Validate(functionNodesCopy, functionLinksCopy);
}


void FunctionGraph::SelectNode(int nodeIndex)
{
	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].Unselect();
	}
	if(nodeIndex != -1)
	{
		functionNodes[nodeIndex].Select();
	}
	if(selectedLinkIndex != -1)
	{
		functionLinks[selectedLinkIndex].Unselect();
	}

	selectedNodeIndex = nodeIndex;
	selectedLinkIndex = -1;
}


void FunctionGraph::SelectLink(int linkIndex)
{
	if(selectedLinkIndex != -1)
	{
		functionLinks[selectedLinkIndex].Unselect();
	}
	if(linkIndex != -1)
	{
		functionLinks[linkIndex].Select();
	}
	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].Unselect();
	}

	selectedLinkIndex = linkIndex;
	selectedNodeIndex = -1;
}


void FunctionGraph::RemoveSelectedNode()
{
	map<int, FunctionNode> functionNodesCopy = functionNodes;
	map<int, FunctionLink> functionLinksCopy = functionLinks;

	vector<int> inputLinkIndices = functionNodesCopy[selectedNodeIndex].GetInputLinkIndices();
	vector<int> outputLinkIndices = functionNodesCopy[selectedNodeIndex].GetOutputLinkIndices();

	for(int i = 0; i < inputLinkIndices.size(); i++)
	{
		int inputLinkIndex = inputLinkIndices[i];
		int inputNodeIndex = functionLinksCopy[inputLinkIndex].GetInputNodeIndex();

		functionNodesCopy[inputNodeIndex].RemoveOutputLink(inputLinkIndex);

		functionLinksCopy.erase(inputLinkIndex);
	}

	for(int i = 0; i < outputLinkIndices.size(); i++)
	{
		int outputLinkIndex = outputLinkIndices[i];
		int outputNodeIndex = functionLinksCopy[outputLinkIndex].GetOutputNodeIndex();

		functionNodesCopy[outputNodeIndex].RemoveInputLink(outputLinkIndex);

		functionLinksCopy.erase(outputLinkIndex);
	}

	functionNodesCopy.erase(selectedNodeIndex);

	Validate(functionNodesCopy, functionLinksCopy);

	selectedNodeIndex = -1;
	selectedLinkIndex = -1;
}


void FunctionGraph::RemoveSelectedLink()
{
	map<int, FunctionNode> functionNodesCopy = functionNodes;
	map<int, FunctionLink> functionLinksCopy = functionLinks;

	int inputNodeIndex = functionLinksCopy[selectedLinkIndex].GetInputNodeIndex();
	int outputNodeIndex = functionLinksCopy[selectedLinkIndex].GetOutputNodeIndex();

	functionNodesCopy[inputNodeIndex].RemoveOutputLink(selectedLinkIndex);
	functionNodesCopy[outputNodeIndex].RemoveInputLink(selectedLinkIndex);

	functionLinksCopy.erase(selectedLinkIndex);

	Validate(functionNodesCopy, functionLinksCopy);

	selectedNodeIndex = -1;
	selectedLinkIndex = -1;
}


void FunctionGraph::RemoveSelected()
{
	if(selectedNodeIndex > 3)
	{
		RemoveSelectedNode();
	}
	else if(selectedLinkIndex != -1)
	{
		RemoveSelectedLink();
	}
}


void FunctionGraph::OnMouseDown(int x, int y)
{
	bool isInteracting = false;

	for(auto it = functionNodes.begin(); it != functionNodes.end() && !isInteracting; it++)
	{
		vector<XMFLOAT2> inputPinPositions = it->second.GetInputPinPositions();

		for(int i = 0; i < inputPinPositions.size() && !isInteracting; i++)
		{
			XMFLOAT2 inputPinPosition = inputPinPositions[i];
			if((x - inputPinPosition.x) * (x - inputPinPosition.x) + (y - inputPinPosition.y) * (y - inputPinPosition.y) <= 256.0f)
			{
				newFunctionLink = make_shared<FunctionLink>(device);
				newFunctionLink->SetOutputNodeIndex(it->first);
				newFunctionLink->SetInputPinIndex(i);
				interactionState = INTERACTION_DRAG_LINK;

				isInteracting = true;
			}
		}
	}

	for(auto it = functionNodes.begin(); it != functionNodes.end() && !isInteracting; it++)
	{
		XMFLOAT2 outputPinPosition = it->second.GetOutputPinPosition();

		if((x - outputPinPosition.x) * (x - outputPinPosition.x) + (y - outputPinPosition.y) * (y - outputPinPosition.y) <= 256.0f)
		{
			newFunctionLink = make_shared<FunctionLink>(device);
			newFunctionLink->SetInputNodeIndex(it->first);
			interactionState = INTERACTION_DRAG_LINK;

			isInteracting = true;
		}

	}

	for(auto it = functionNodes.begin(); it != functionNodes.end() && !isInteracting; it++)
	{
		XMFLOAT2 position = it->second.GetPosition();

		if(x >= position.x - 64.0f && x <= position.x + 64.0f && y >= position.y - 64.0f && y <= position.y + 64.0f)
		{
			SelectNode(it->first);
			clickOffset = XMFLOAT2(position.x - x, position.y - y);
			interactionState = INTERACTION_DRAG_NODE;

			isInteracting = true;
		}
	}

	for(auto it = functionLinks.begin(); it != functionLinks.end() && !isInteracting; it++)
	{
		vector<XMFLOAT2> positions = it->second.GetPolylineVertices();

		for(int i = 0; i < 3; i++)
		{
			XMFLOAT2 a = positions[i];
			XMFLOAT2 b = positions[i + 1];

			if(abs((x - a.x) / (b.x - a.x) - (y - a.y) / (b.y - a.y)) < 0.2f)
			{
				SelectLink(it->first);

				isInteracting = true;
			}
		}
	}

	if(!isInteracting)
	{
		SelectNode(-1);
		interactionState = INTERACTION_NONE;
	}
}


void FunctionGraph::OnMouseMove(int x, int y)
{
	switch(interactionState)
	{
		case INTERACTION_DRAG_NODE:
		{
			functionNodes[selectedNodeIndex].SetPosition(x + clickOffset.x, y + clickOffset.y);

			vector<int> inputLinkIndices = functionNodes[selectedNodeIndex].GetInputLinkIndices();
			for(int i = 0; i < inputLinkIndices.size(); i++)
			{
				if(inputLinkIndices[i] != -1)
				{
					XMFLOAT2 p0 = functionNodes[functionLinks[inputLinkIndices[i]].GetInputNodeIndex()].GetOutputPinPosition();
					XMFLOAT2 p1 = functionNodes[selectedNodeIndex].GetInputPinPositions()[i];
					functionLinks[inputLinkIndices[i]].SetPolylineCoords(p0, p1, COLOR);
				}
			}

			vector<int> outputLinkIndices = functionNodes[selectedNodeIndex].GetOutputLinkIndices();
			for(int i = 0; i < outputLinkIndices.size(); i++)
			{
				if(outputLinkIndices[i] != -1)
				{
					XMFLOAT2 p0 = functionNodes[selectedNodeIndex].GetOutputPinPosition();
					XMFLOAT2 p1 = functionNodes[functionLinks[outputLinkIndices[i]].GetOutputNodeIndex()].GetInputPinPositions()[functionLinks[outputLinkIndices[i]].GetInputPinIndex()];
					functionLinks[outputLinkIndices[i]].SetPolylineCoords(p0, p1, COLOR);
				}
			}

			break;
		}
		case INTERACTION_DRAG_LINK:
		{
			int inputNodeIndex = newFunctionLink->GetInputNodeIndex();
			int outputNodeIndex = newFunctionLink->GetOutputNodeIndex();

			XMFLOAT2 p0;
			XMFLOAT2 p1;

			if(inputNodeIndex == -1)
			{
				p0 = XMFLOAT2(x, y);
				p1 = functionNodes[outputNodeIndex].GetInputPinPositions()[newFunctionLink->GetInputPinIndex()];
			}
			else
			{
				p0 = functionNodes[inputNodeIndex].GetOutputPinPosition();
				p1 = XMFLOAT2(x, y);
			}

			newFunctionLink->SetPolylineCoords(p0, p1, COLOR);
			break;
		}
		default:
		{
			break;
		}
	}
}


void FunctionGraph::OnMouseUp(int x, int y)
{
	if(interactionState == INTERACTION_DRAG_LINK)
	{
		int inputNodeIndex = newFunctionLink->GetInputNodeIndex();
		int outputNodeIndex = newFunctionLink->GetOutputNodeIndex();

		XMFLOAT2 p0;
		XMFLOAT2 p1;

		if(inputNodeIndex == -1)
		{
			bool inputPinIsFound = false;

			for(auto it = functionNodes.begin(); it != functionNodes.end() && !inputPinIsFound; it++)
			{
				XMFLOAT2 outputPinPosition = it->second.GetOutputPinPosition();

				if((x - outputPinPosition.x) * (x - outputPinPosition.x) + (y - outputPinPosition.y) * (y - outputPinPosition.y) <= 144.0f)
				{
					AddLink(it->first, outputNodeIndex, newFunctionLink->GetInputPinIndex());
					inputPinIsFound = true;
				}
			}
		}
		else
		{
			bool inputPinIsFound = false;

			for(auto it = functionNodes.begin(); it != functionNodes.end() && !inputPinIsFound; it++)
			{
				vector<XMFLOAT2> inputPinPositions = it->second.GetInputPinPositions();

				for(int i = 0; i < inputPinPositions.size() && !inputPinIsFound; i++)
				{
					XMFLOAT2 inputPinPosition = inputPinPositions[i];

					if((x - inputPinPosition.x) * (x - inputPinPosition.x) + (y - inputPinPosition.y) * (y - inputPinPosition.y) <= 256.0f)
					{
						AddLink(inputNodeIndex, it->first, i);
						inputPinIsFound = true;
					}
				}
			}
		}
	}

	newFunctionLink = nullptr;
	interactionState = INTERACTION_NONE;
}


void FunctionGraph::Process()
{
	for(int i = 0; i < graphTraverseOrder.size(); i++)
	{
		FunctionNode &currentNode = functionNodes[graphTraverseOrder[i]];
		
		if(!currentNode.IsCalculated())
		{
			vector<int> inputLinkIndices = currentNode.GetInputLinkIndices();

			vector<TextureMemoryPtr> textureMemoryPtrs = vector<TextureMemoryPtr>(inputLinkIndices.size());
			for(int j = 0; j < inputLinkIndices.size(); j++)
			{
				if(inputLinkIndices[j] != -1)
				{
					textureMemoryPtrs[j] = functionNodes[functionLinks[inputLinkIndices[j]].GetInputNodeIndex()].GetTextureMemory();
				}
				else
				{
					textureMemoryPtrs[j] = nullptr;
				}
			}

			vector<int> intParameters = currentNode.GetIntParameters();
			vector<float> floatParameters = currentNode.GetFloatParameters();

			int functionIndex = currentNode.GetFunctionIndex();

			TextureMemoryPtr generatedTextureMemoryPtr = Function(functionIndex, textureMemoryPtrs, intParameters, floatParameters);
			currentNode.SetTextureMemory(generatedTextureMemoryPtr);

			currentNode.MarkAsCalculated();

			vector<int> outputLinkIndices = currentNode.GetOutputLinkIndices();
			for(int j = 0; j < outputLinkIndices.size(); j++)
			{
				functionNodes[functionLinks[outputLinkIndices[j]].GetOutputNodeIndex()].RequestCalculation();
			}
		}
	}
}


void FunctionGraph::Render()
{
	if(newFunctionLink.get() != nullptr) newFunctionLink->Render();

	for(auto it = functionLinks.begin(); it != functionLinks.end(); it++)
	{
		it->second.Render();
	}

	for(auto it = functionNodes.begin(); it != functionNodes.end(); it++)
	{
		it->second.Render();
	}
}


void FunctionGraph::Release()
{
	functionNodes.clear();
	functionLinks.clear();
}
