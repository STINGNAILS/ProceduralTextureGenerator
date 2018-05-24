#include "stdafx.h"
#include "FunctionGraph.h"


FunctionGraph::FunctionGraph()
{
	nextNodeIndex = 0;
	selectedNodeIndex = -1;
	trackedNodeIndex = 0;
	functionNodesPtr = make_shared<map<int, FunctionNode>>();

	nextLinkIndex = 0;
	selectedLinkIndex = -1;
	functionLinksPtr = make_shared<map<int, FunctionLink>>();

	graphTraverseOrderPtr = make_shared<vector<int>>();

	graphWasChanged = false;

	newFunctionLink = nullptr;

	interactionState = INTERACTION_NONE;

	hoveredPortDescriptor.hoveredPortFunctionIndex = -1;
	hoveredPortDescriptor.hoveredPortIndex = -1;
	hoveredPortDescriptor.position = XMFLOAT2(0.0f, 0.0f);
}


FunctionGraph::~FunctionGraph()
{
	
}


void FunctionGraph::BindDummyObject(shared_ptr<DummyObject> dummyObjectPtr_)
{
	dummyObjectPtr = dummyObjectPtr_;

	UpdateDummyObjectTexture();
}


void FunctionGraph::BindTextureQuad(shared_ptr<TextureQuad> textureQuadPtr_)
{
	textureQuadPtr = textureQuadPtr_;

	UpdateTextureQuadTexture();
}


int FunctionGraph::GetSelectedNodeIndex()
{
	return selectedNodeIndex;
}


int FunctionGraph::GetSelectedNodeFunctionIndex()
{
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();

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
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();

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
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();

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


XMFLOAT4 FunctionGraph::GetScope()
{
	XMFLOAT2 position = functionNodesPtr->begin()->second.GetPosition();

	float xMin = position.x;
	float yMin = position.y;
	float xMax = position.x;
	float yMax = position.y;

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end(); it++)
	{
		XMFLOAT2 position = it->second.GetPosition();

		if(position.x < xMin)
		{
			xMin = position.x;
		}
		else if(position.x > xMax)
		{
			xMax = position.x;
		}

		if(position.y < yMin)
		{
			yMin = position.y;
		}
		else if(position.y > yMax)
		{
			yMax = position.y;
		}
	}

	return XMFLOAT4(xMin - 32.0f, yMin - 32.0f, xMax + 32.0f, yMax + 32.0f);
}


HoveredPortDescriptor FunctionGraph::GetHoveredPortDescriptor()
{
	return hoveredPortDescriptor;
}


void FunctionGraph::SetSelectedNodeIntParameter(int parameterIndex, int value)
{
	lock_guard<mutex> lock(functionGraphMutex);

	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();

	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].SetIntParameter(parameterIndex, value);
	}
}


void FunctionGraph::SetSelectedNodeFloatParameter(int parameterIndex, float value)
{
	lock_guard<mutex> lock(functionGraphMutex);

	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();

	if(selectedNodeIndex != -1)
	{
		functionNodes[selectedNodeIndex].SetFloatParameter(parameterIndex, value);
	}
}


bool FunctionGraph::GraphWasChanged()
{
	return graphWasChanged;
}


enum NodeColor
{
	WHITE,
	GRAY,
	BLACK
};


void FunctionGraph::Validate(shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr, shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr)
{
	map<int, FunctionNode> &functionNodesCopy = *functionNodesCopyPtr.get();
	map<int, FunctionLink> &functionLinksCopy = *functionLinksCopyPtr.get();

	shared_ptr<vector<int>> graphTraverseOrderCopyPtr = make_shared<vector<int>>();
	vector<int> &graphTraverseOrderCopy = *graphTraverseOrderCopyPtr.get();

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

				bool inputsAreBlack = true;

				for(int i = 0; i < inputLinkIndices.size(); i++)
				{
					if(inputLinkIndices[i] != -1)
					{
						int inputNodeIndex = functionLinksCopy[inputLinkIndices[i]].GetInputNodeIndex();

						if(nodeColors[inputNodeIndex] == WHITE)
						{
							traverseStack.push(inputNodeIndex);
							nodeColors[inputNodeIndex] = GRAY;
							inputsAreBlack = false;
							break;
						}
						else if(nodeColors[inputNodeIndex] == GRAY)
						{
							isDirectedAcyclic = false;
							break;
						}
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
		unique_lock<mutex> lock(functionGraphMutex);

		functionNodesPtr = functionNodesCopyPtr;
		functionLinksPtr = functionLinksCopyPtr;
		graphTraverseOrderPtr = graphTraverseOrderCopyPtr;

		lock.unlock();
	}
}


void FunctionGraph::UpdateDummyObjectTexture()
{
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();
	dummyObjectPtr->SetBaseColorMap(functionNodes[0].GetDirectXTexture());
	dummyObjectPtr->SetMetallicMap(functionNodes[1].GetDirectXTexture());
	dummyObjectPtr->SetRoughnessMap(functionNodes[2].GetDirectXTexture());
	dummyObjectPtr->SetNormalMap(functionNodes[3].GetDirectXTexture());
}


void FunctionGraph::UpdateTextureQuadTexture()
{
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();
	textureQuadPtr->SetTexture(functionNodes[trackedNodeIndex].GetDirectXTexture());
}


void FunctionGraph::AddNode(int functionIndex, float x, float y)
{
	shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr = make_shared<map<int, FunctionNode>>();
	shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr = make_shared<map<int, FunctionLink>>();

	unique_lock<mutex> lock(functionGraphMutex);

	*functionNodesCopyPtr.get() = *functionNodesPtr.get();
	*functionLinksCopyPtr.get() = *functionLinksPtr.get();

	lock.unlock();

	map<int, FunctionNode> &functionNodesCopy = *functionNodesCopyPtr.get();
	map<int, FunctionLink> &functionLinksCopy = *functionLinksCopyPtr.get();

	FunctionNode newFunctionNode = FunctionNode(functionIndex);
	newFunctionNode.SetPosition(x, y);

	functionNodesCopy[nextNodeIndex] = newFunctionNode;

	nextNodeIndex++;

	Validate(functionNodesCopyPtr, functionLinksCopyPtr);
}


void FunctionGraph::AddLink(int inputNodeIndex, int outputNodeIndex, int inputPortIndex)
{
	shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr = make_shared<map<int, FunctionNode>>();
	shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr = make_shared<map<int, FunctionLink>>();
	
	unique_lock<mutex> lock(functionGraphMutex);

	*functionNodesCopyPtr.get() = *functionNodesPtr.get();
	*functionLinksCopyPtr.get() = *functionLinksPtr.get();

	lock.unlock();

	map<int, FunctionNode> &functionNodesCopy = *functionNodesCopyPtr.get();
	map<int, FunctionLink> &functionLinksCopy = *functionLinksCopyPtr.get();
	

	int linkToRemoveIndex = functionNodesCopy[outputNodeIndex].GetInputLinkIndices()[inputPortIndex];
	if(linkToRemoveIndex != -1)
	{
		functionNodesCopy[functionLinksCopy[linkToRemoveIndex].GetInputNodeIndex()].RemoveOutputLink(linkToRemoveIndex);
		functionNodesCopy[functionLinksCopy[linkToRemoveIndex].GetOutputNodeIndex()].RemoveOutputLink(linkToRemoveIndex);

		functionLinksCopy.erase(linkToRemoveIndex);
	}

	functionLinksCopy[nextLinkIndex] = FunctionLink();
	functionLinksCopy[nextLinkIndex].SetInputNodeIndex(inputNodeIndex);
	functionLinksCopy[nextLinkIndex].SetOutputNodeIndex(outputNodeIndex);
	functionLinksCopy[nextLinkIndex].SetInputPortIndex(inputPortIndex);
	functionLinksCopy[nextLinkIndex].SetPolylineCoords(functionNodesCopy[inputNodeIndex].GetOutputPortPosition(), functionNodesCopy[outputNodeIndex].GetInputPortPositions()[inputPortIndex], COLOR);

	functionNodesCopy[inputNodeIndex].AddOutputLink(nextLinkIndex);
	functionNodesCopy[outputNodeIndex].AddInputLink(inputPortIndex, nextLinkIndex);

	nextLinkIndex++;

	Validate(functionNodesCopyPtr, functionLinksCopyPtr);
}


void FunctionGraph::SelectNode(int nodeIndex)
{
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();
	map<int, FunctionLink> &functionLinks = *functionLinksPtr.get();

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
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();
	map<int, FunctionLink> &functionLinks = *functionLinksPtr.get();

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
	shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr = make_shared<map<int, FunctionNode>>();
	shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr = make_shared<map<int, FunctionLink>>();

	unique_lock<mutex> lock(functionGraphMutex);

	*functionNodesCopyPtr.get() = *functionNodesPtr.get();
	*functionLinksCopyPtr.get() = *functionLinksPtr.get();

	lock.unlock();

	map<int, FunctionNode> &functionNodesCopy = *functionNodesCopyPtr.get();
	map<int, FunctionLink> &functionLinksCopy = *functionLinksCopyPtr.get();

	vector<int> inputLinkIndices = functionNodesCopy[selectedNodeIndex].GetInputLinkIndices();
	vector<int> outputLinkIndices = functionNodesCopy[selectedNodeIndex].GetOutputLinkIndices();

	for(int i = 0; i < inputLinkIndices.size(); i++)
	{
		int inputLinkIndex = inputLinkIndices[i];

		if(inputLinkIndex != -1)
		{
			int inputNodeIndex = functionLinksCopy[inputLinkIndex].GetInputNodeIndex();

			functionNodesCopy[inputNodeIndex].RemoveOutputLink(inputLinkIndex);
			functionLinksCopy.erase(inputLinkIndex);
		}
	}

	for(int i = 0; i < outputLinkIndices.size(); i++)
	{
		int outputLinkIndex = outputLinkIndices[i];
		int outputNodeIndex = functionLinksCopy[outputLinkIndex].GetOutputNodeIndex();

		functionNodesCopy[outputNodeIndex].RemoveInputLink(outputLinkIndex);

		functionLinksCopy.erase(outputLinkIndex);
	}

	functionNodesCopy.erase(selectedNodeIndex);

	Validate(functionNodesCopyPtr, functionLinksCopyPtr);

	if(trackedNodeIndex == selectedNodeIndex)
	{
		trackedNodeIndex = 0;
	}

	selectedNodeIndex = -1;
	selectedLinkIndex = -1;
}


void FunctionGraph::RemoveSelectedLink()
{
	shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr = make_shared<map<int, FunctionNode>>();
	shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr = make_shared<map<int, FunctionLink>>();

	unique_lock<mutex> lock(functionGraphMutex);

	*functionNodesCopyPtr.get() = *functionNodesPtr.get();
	*functionLinksCopyPtr.get() = *functionLinksPtr.get();

	lock.unlock();

	map<int, FunctionNode> &functionNodesCopy = *functionNodesCopyPtr.get();
	map<int, FunctionLink> &functionLinksCopy = *functionLinksCopyPtr.get();

	int inputNodeIndex = functionLinksCopy[selectedLinkIndex].GetInputNodeIndex();
	int outputNodeIndex = functionLinksCopy[selectedLinkIndex].GetOutputNodeIndex();

	functionNodesCopy[inputNodeIndex].RemoveOutputLink(selectedLinkIndex);
	functionNodesCopy[outputNodeIndex].RemoveInputLink(selectedLinkIndex);

	functionLinksCopy.erase(selectedLinkIndex);

	Validate(functionNodesCopyPtr, functionLinksCopyPtr);

	selectedNodeIndex = -1;
	selectedLinkIndex = -1;
}


void FunctionGraph::RemoveSelected()
{
	if(selectedNodeIndex > 3)
	{
		RemoveSelectedNode();
		graphWasChanged = true;
	}
	else if(selectedLinkIndex != -1)
	{
		RemoveSelectedLink();
		graphWasChanged = true;
	}
	else
	{
		graphWasChanged = false;
	}
}


void FunctionGraph::OnMouseDown(int x, int y)
{
	bool isInteracting = false;

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !isInteracting; it++)
	{
		vector<XMFLOAT2> inputPortPositions = it->second.GetInputPortPositions();

		for(int i = 0; i < inputPortPositions.size() && !isInteracting; i++)
		{
			XMFLOAT2 inputPortPosition = inputPortPositions[i];
			if((x - inputPortPosition.x) * (x - inputPortPosition.x) + (y - inputPortPosition.y) * (y - inputPortPosition.y) <= 36.0f)
			{
				newFunctionLink = make_shared<FunctionLink>();
				newFunctionLink->SetOutputNodeIndex(it->first);
				newFunctionLink->SetInputPortIndex(i);
				interactionState = INTERACTION_DRAG_LINK;

				isInteracting = true;
			}
		}
	}

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !isInteracting; it++)
	{
		XMFLOAT2 outputPortPosition = it->second.GetOutputPortPosition();

		if((x - outputPortPosition.x) * (x - outputPortPosition.x) + (y - outputPortPosition.y) * (y - outputPortPosition.y) <= 36.0f)
		{
			newFunctionLink = make_shared<FunctionLink>();
			newFunctionLink->SetInputNodeIndex(it->first);
			interactionState = INTERACTION_DRAG_LINK;

			isInteracting = true;
		}

	}

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !isInteracting; it++)
	{
		XMFLOAT2 position = it->second.GetPosition();

		if(x >= position.x - 32.0f && x <= position.x + 32.0f && y >= position.y - 32.0f && y <= position.y + 32.0f)
		{
			SelectNode(it->first);
			clickOffset = XMFLOAT2(position.x - x, position.y - y);
			interactionState = INTERACTION_DRAG_NODE;

			isInteracting = true;
		}
	}

	for(auto it = functionLinksPtr->begin(); it != functionLinksPtr->end() && !isInteracting; it++)
	{
		vector<XMFLOAT2> positions = it->second.GetPolylineVertices();

		for(int i = 0; i < 3; i++)
		{
			XMFLOAT2 p1 = positions[i];
			XMFLOAT2 p2 = positions[i + 1];

			float a = p1.y - p2.y;
			float b = p2.x - p1.x;
			float c = p1.x * p2.y - p2.x * p1.y;

			float x_;
			float y_;

			if(abs(a) < 1e-6f)
			{
				x_ = x;
				y_ = -c / b;
			}
			else if(abs(b) < 1e-6f)
			{
				x_ = -c / a;
				y_ = y;
			}
			else
			{
				float d = -b * x + a * y;

				x_ = -(a * c + b * d) / (a * a + b * b);
				y_ = (b * x_ + d) / a;
			}

			if(abs(p1.x - p2.x) > (abs(x_ - p1.x) + abs(x_ - p2.x) - 1.0f) && abs(p1.y - p2.y) > (abs(y_ - p1.y) + abs(y_ - p2.y) - 1.0f))
			{
				if((x - x_) * (x - x_) + (y - y_) * (y - y_) < 9.0f)
				{
					SelectLink(it->first);

					isInteracting = true;
				}
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
	map<int, FunctionNode> &functionNodes = *functionNodesPtr.get();
	map<int, FunctionLink> &functionLinks = *functionLinksPtr.get();

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
					XMFLOAT2 p0 = functionNodes[functionLinks[inputLinkIndices[i]].GetInputNodeIndex()].GetOutputPortPosition();
					XMFLOAT2 p1 = functionNodes[selectedNodeIndex].GetInputPortPositions()[i];
					functionLinks[inputLinkIndices[i]].SetPolylineCoords(p0, p1, COLOR);
				}
			}

			vector<int> outputLinkIndices = functionNodes[selectedNodeIndex].GetOutputLinkIndices();
			for(int i = 0; i < outputLinkIndices.size(); i++)
			{
				if(outputLinkIndices[i] != -1)
				{
					XMFLOAT2 p0 = functionNodes[selectedNodeIndex].GetOutputPortPosition();
					XMFLOAT2 p1 = functionNodes[functionLinks[outputLinkIndices[i]].GetOutputNodeIndex()].GetInputPortPositions()[functionLinks[outputLinkIndices[i]].GetInputPortIndex()];
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
				p1 = functionNodes[outputNodeIndex].GetInputPortPositions()[newFunctionLink->GetInputPortIndex()];
			}
			else
			{
				p0 = functionNodes[inputNodeIndex].GetOutputPortPosition();
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

	hoveredPortDescriptor.hoveredPortFunctionIndex = -1;

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && hoveredPortDescriptor.hoveredPortFunctionIndex == -1; it++)
	{
		vector<XMFLOAT2> inputPortPositions = it->second.GetInputPortPositions();

		for(int i = 0; i < inputPortPositions.size() && hoveredPortDescriptor.hoveredPortFunctionIndex == -1; i++)
		{
			XMFLOAT2 inputPortPosition = inputPortPositions[i];
			if((x - inputPortPosition.x) * (x - inputPortPosition.x) + (y - inputPortPosition.y) * (y - inputPortPosition.y) <= 36.0f)
			{
				hoveredPortDescriptor.hoveredPortFunctionIndex = it->second.GetFunctionIndex();
				hoveredPortDescriptor.hoveredPortIndex = i;
				hoveredPortDescriptor.position = inputPortPosition;
			}
		}
	}
}


void FunctionGraph::OnMouseUp(int x, int y)
{
	if(interactionState == INTERACTION_DRAG_LINK)
	{
		int inputNodeIndex = newFunctionLink->GetInputNodeIndex();
		int outputNodeIndex = newFunctionLink->GetOutputNodeIndex();
		int inputPortIndex = -1;

		bool portIsFound = false;

		XMFLOAT2 p0;
		XMFLOAT2 p1;

		if(inputNodeIndex == -1)
		{
			for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !portIsFound; it++)
			{
				XMFLOAT2 outputPortPosition = it->second.GetOutputPortPosition();

				if((x - outputPortPosition.x) * (x - outputPortPosition.x) + (y - outputPortPosition.y) * (y - outputPortPosition.y) <= 36.0f)
				{
					inputNodeIndex = it->first;
					inputPortIndex = newFunctionLink->GetInputPortIndex();
					portIsFound = true;
				}
			}
		}
		else
		{
			for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !portIsFound; it++)
			{
				vector<XMFLOAT2> inputPortPositions = it->second.GetInputPortPositions();

				for(int i = 0; i < inputPortPositions.size() && !portIsFound; i++)
				{
					XMFLOAT2 inputPortPosition = inputPortPositions[i];

					if((x - inputPortPosition.x) * (x - inputPortPosition.x) + (y - inputPortPosition.y) * (y - inputPortPosition.y) <= 36.0f)
					{
						outputNodeIndex = it->first;
						inputPortIndex = i;
						portIsFound = true;
					}
				}
			}
		}

		if(inputNodeIndex != -1 && outputNodeIndex != -1)
		{
			AddLink(inputNodeIndex, outputNodeIndex, inputPortIndex);
			graphWasChanged = (functionLinksPtr->find(nextLinkIndex - 1) != functionLinksPtr->end());
		}
		else
		{
			graphWasChanged = false;
		}
	}
	else
	{
		graphWasChanged = false;
	}

	newFunctionLink = nullptr;
	interactionState = INTERACTION_NONE;
}


void FunctionGraph::OnMouseDoubleClick(int x, int y)
{
	bool isInteracting = false;

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end() && !isInteracting; it++)
	{
		XMFLOAT2 position = it->second.GetPosition();

		if(x >= position.x - 32.0f && x <= position.x + 32.0f && y >= position.y - 32.0f && y <= position.y + 32.0f)
		{
			SelectNode(it->first);

			trackedNodeIndex = it->first;

			UpdateTextureQuadTexture();
		}
	}
}


void FunctionGraph::Process()
{
	abortProcessing = false;

	int graphSize = graphTraverseOrderPtr->size();
	for(int i = 0; i < graphSize && !abortProcessing; i++)
	{
		unique_lock<mutex> lock1(functionGraphMutex);

		map<int, FunctionNode> &functionNodesInput = *functionNodesPtr.get();
		map<int, FunctionLink> &functionLinksInput = *functionLinksPtr.get();
		vector<int> &graphTraverseOrder = *graphTraverseOrderPtr.get();

		int currentNodeIndex = graphTraverseOrder[i];
		FunctionNode &currentNode = functionNodesInput[currentNodeIndex];

		vector<int> inputLinkIndices = currentNode.GetInputLinkIndices();

		vector<TextureMemoryPtr> textureMemoryPtrs(inputLinkIndices.size());
		for(int j = 0; j < inputLinkIndices.size(); j++)
		{
			if(inputLinkIndices[j] != -1)
			{
				textureMemoryPtrs[j] = functionNodesInput[functionLinksInput[inputLinkIndices[j]].GetInputNodeIndex()].GetTextureMemory();
			}
			else
			{
				textureMemoryPtrs[j] = nullptr;
			}
		}

		vector<int> intParameters = currentNode.GetIntParameters();
		vector<float> floatParameters = currentNode.GetFloatParameters();

		int functionIndex = currentNode.GetFunctionIndex();

		CalculatedState calculatedState = currentNode.GetCalculatedState();
		if(calculatedState == NOT_CALCULATED)
		{
			currentNode.SetCalculatedState(BEING_CALCULATED);
		}

		lock1.unlock();

		if(calculatedState == NOT_CALCULATED)
		{
			TextureMemoryPtr generatedTextureMemoryPtr = Function(functionIndex, textureMemoryPtrs, intParameters, floatParameters);

			unique_lock<mutex> lock2(functionGraphMutex);

			map<int, FunctionNode> &functionNodesOutput = *functionNodesPtr.get();
			map<int, FunctionLink> &functionLinksOutput = *functionLinksPtr.get();

			auto it = functionNodesOutput.find(currentNodeIndex);
			if(it != functionNodesOutput.end())
			{
				if(it->second.GetCalculatedState() == BEING_CALCULATED)
				{
					it->second.SetTextureMemory(generatedTextureMemoryPtr);

					it->second.SetCalculatedState(CALCULATED);

					vector<int> outputLinkIndices = it->second.GetOutputLinkIndices();
					for(int j = 0; j < outputLinkIndices.size(); j++)
					{
						FunctionLink &functionLink = functionLinksOutput[outputLinkIndices[j]];
						int outputNodeIndex = functionLink.GetOutputNodeIndex();
						int inputSlotIndex = functionLink.GetInputPortIndex();

						functionNodesOutput[outputNodeIndex].SetCalculatedState(NOT_CALCULATED);
						functionNodesOutput[outputNodeIndex].SetInputPortTextureType(inputSlotIndex, generatedTextureMemoryPtr->GetTextureType());
					}

					UpdateDummyObjectTexture();
					UpdateTextureQuadTexture();
				}
			}

			lock2.unlock();
		}
	}
}


void FunctionGraph::AbortProcessing()
{
	abortProcessing = true;
}


void FunctionGraph::Render()
{
	shared_ptr<map<int, FunctionNode>> functionNodesToRenderPtr;
	shared_ptr<map<int, FunctionLink>> functionLinksToRenderPtr;

	unique_lock<mutex> lock(functionGraphMutex);

	functionNodesToRenderPtr = functionNodesPtr;
	functionLinksToRenderPtr = functionLinksPtr;

	lock.unlock();

	if(newFunctionLink.get() != nullptr) newFunctionLink->Render();

	for(auto it = functionLinksToRenderPtr->begin(); it != functionLinksToRenderPtr->end(); it++)
	{
		it->second.Render();
	}

	for(auto it = functionNodesToRenderPtr->begin(); it != functionNodesToRenderPtr->end(); it++)
	{
		it->second.Render();
	}
}


void FunctionGraph::Reset()
{
	nextNodeIndex = 0;
	selectedNodeIndex = -1;
	trackedNodeIndex = 0;
	functionNodesPtr = make_shared<map<int, FunctionNode>>();

	nextLinkIndex = 0;
	selectedLinkIndex = -1;
	functionLinksPtr = make_shared<map<int, FunctionLink>>();

	graphTraverseOrderPtr = make_shared<vector<int>>();

	graphWasChanged = false;

	AddNode(0, -192.0f, 0.0f);
	AddNode(1, -64.0f, 0.0f);
	AddNode(2, 64.0f, 0.0f);
	AddNode(3, 192.0f, 0.0f);

	newFunctionLink = nullptr;

	interactionState = INTERACTION_NONE;
}


void FunctionGraph::SaveToFile(LPSTR fileName)
{
	lock_guard<mutex> lock(functionGraphMutex);

	ofstream os;
	os.open(fileName, ios::binary);
	if(!os.is_open())
	{
		return;
	}

	int nodesNum = functionNodesPtr->size();
	os.write((char*) &nodesNum, sizeof(int));

	int linksNum = functionLinksPtr->size();
	os.write((char*) &linksNum, sizeof(int));

	os.write((char*) &nextNodeIndex, sizeof(int));

	os.write((char*) &nextLinkIndex, sizeof(int));

	for(auto it = functionNodesPtr->begin(); it != functionNodesPtr->end(); it++)
	{
		int nodeIndex = it->first;
		os.write((char*) &nodeIndex, sizeof(int));

		int functionIndex = it->second.GetFunctionIndex();
		os.write((char*) &functionIndex, sizeof(int));

		vector<int> inputLinkIndices = it->second.GetInputLinkIndices();
		int inputLinksNum = inputLinkIndices.size();
		os.write((char*) &inputLinksNum, sizeof(int));
		for(int i = 0; i < inputLinksNum; i++)
		{
			int inputLinkIndex = inputLinkIndices[i];
			os.write((char*) &inputLinkIndex, sizeof(int));
		}

		vector<int> outputLinkIndices = it->second.GetOutputLinkIndices();
		int outputLinksNum = outputLinkIndices.size();
		os.write((char*) &outputLinksNum, sizeof(int));
		for(int i = 0; i < outputLinksNum; i++)
		{
			int outputLinkIndex = outputLinkIndices[i];
			os.write((char*) &outputLinkIndex, sizeof(int));
		}

		vector<int> intParameters = it->second.GetIntParameters();
		int intParametersNum = intParameters.size();
		os.write((char*) &intParametersNum, sizeof(int));
		for(int i = 0; i < intParametersNum; i++)
		{
			int intParameter = intParameters[i];
			os.write((char*) &intParameter, sizeof(int));
		}

		vector<float> floatParameters = it->second.GetFloatParameters();
		int floatParametersNum = floatParameters.size();
		os.write((char*) &floatParametersNum, sizeof(int));
		for(int i = 0; i < floatParametersNum; i++)
		{
			float floatParameter = floatParameters[i];
			os.write((char*) &floatParameter, sizeof(float));
		}

		XMFLOAT2 position = it->second.GetPosition();
		os.write((char*) &position, sizeof(XMFLOAT2));
	}

	for(auto it = functionLinksPtr->begin(); it != functionLinksPtr->end(); it++)
	{
		int linkIndex = it->first;
		os.write((char*) &linkIndex, sizeof(int));

		int inputNodeIndex = it->second.GetInputNodeIndex();
		os.write((char*) &inputNodeIndex, sizeof(int));

		int outputNodeIndex = it->second.GetOutputNodeIndex();
		os.write((char*) &outputNodeIndex, sizeof(int));

		int inputPortIndex = it->second.GetInputPortIndex();
		os.write((char*) &inputPortIndex, sizeof(int));
	}

	os.close();
}


void FunctionGraph::LoadFromFile(LPSTR fileName)
{
	shared_ptr<map<int, FunctionNode>> functionNodesToLoadPtr = make_shared<map<int, FunctionNode>>();
	shared_ptr<map<int, FunctionLink>> functionLinksToLoadPtr = make_shared<map<int, FunctionLink>>();

	map<int, FunctionNode> &functionNodesToLoad = *functionNodesToLoadPtr.get();
	map<int, FunctionLink> &functionLinksToLoad = *functionLinksToLoadPtr.get();

	ifstream is;
	is.open(fileName, ios::binary);
	if(!is.is_open())
	{
		return;
	}

	int nodesNum = 0;
	is.read((char*) &nodesNum, sizeof(int));

	int linksNum = 0;
	is.read((char*) &linksNum, sizeof(int));

	is.read((char*) &nextNodeIndex, sizeof(int));

	is.read((char*) &nextLinkIndex, sizeof(int));

	for(int k = 0; k < nodesNum; k++)
	{
		int nodeIndex = -1;
		is.read((char*) &nodeIndex, sizeof(int));

		int functionIndex = -1;
		is.read((char*) &functionIndex, sizeof(int));

		FunctionNode functionNode = FunctionNode(functionIndex);

		int inputLinksNum = 0;
		is.read((char*) &inputLinksNum, sizeof(int));
		for(int i = 0; i < inputLinksNum; i++)
		{
			int inputLinkIndex = -1;
			is.read((char*) &inputLinkIndex, sizeof(int));
			if(inputLinkIndex != -1)
			{
				functionNode.AddInputLink(i, inputLinkIndex);
			}
		}

		int outputLinksNum = 0;
		is.read((char*) &outputLinksNum, sizeof(int));
		for(int i = 0; i < outputLinksNum; i++)
		{
			int outputLinkIndex = -1;
			is.read((char*) &outputLinkIndex, sizeof(int));
			functionNode.AddOutputLink(outputLinkIndex);
		}

		int intParametersNum = 0;
		is.read((char*) &intParametersNum, sizeof(int));
		for(int i = 0; i < intParametersNum; i++)
		{
			int intParameter = 0;
			is.read((char*) &intParameter, sizeof(int));
			functionNode.SetIntParameter(i, intParameter);
		}

		int floatParametersNum = 0;
		is.read((char*) &floatParametersNum, sizeof(int));
		for(int i = 0; i < floatParametersNum; i++)
		{
			float floatParameter = 0.0f;
			is.read((char*) &floatParameter, sizeof(float));
			functionNode.SetFloatParameter(i, floatParameter);
		}

		XMFLOAT2 position = XMFLOAT2(0.0f, 0.0f);
		is.read((char*) &position, sizeof(XMFLOAT2));
		functionNode.SetPosition(position.x, position.y);

		functionNodesToLoad[nodeIndex] = functionNode;
	}

	for(int k = 0; k < linksNum; k++)
	{
		int linkIndex = -1;
		is.read((char*) &linkIndex, sizeof(int));

		FunctionLink functionLink;

		int inputNodeIndex = -1;
		is.read((char*) &inputNodeIndex, sizeof(int));
		functionLink.SetInputNodeIndex(inputNodeIndex);

		int outputNodeIndex = -1;
		is.read((char*) &outputNodeIndex, sizeof(int));
		functionLink.SetOutputNodeIndex(outputNodeIndex);

		int inputPortIndex = -1;
		is.read((char*) &inputPortIndex, sizeof(int));
		functionLink.SetInputPortIndex(inputPortIndex);

		functionLink.SetPolylineCoords(functionNodesToLoad[inputNodeIndex].GetOutputPortPosition(), functionNodesToLoad[outputNodeIndex].GetInputPortPositions()[inputPortIndex], COLOR);

		functionLinksToLoad[linkIndex] = functionLink;
	}

	is.close();

	Validate(functionNodesToLoadPtr, functionLinksToLoadPtr);

	selectedNodeIndex = -1;
	selectedNodeIndex = -1;
	trackedNodeIndex = 0;

	graphWasChanged = false;

	UpdateDummyObjectTexture();
	UpdateTextureQuadTexture();
}