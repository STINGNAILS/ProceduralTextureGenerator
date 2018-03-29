#pragma once

#include "RenderableObject.h"
#include "FunctionNode.h"
#include "Function.h"


enum InteractionState
{
	INTERACTION_NONE,
	INTERACTION_DRAG_NODE,
	INTERACTION_DRAG_LINK
};


class FunctionGraph : public RenderableObject
{
	private:

	int nextNodeIndex;
	int selectedNodeIndex;
	shared_ptr<map<int, FunctionNode>> functionNodesPtr;

	int nextLinkIndex;
	int selectedLinkIndex;
	shared_ptr<map<int, FunctionLink>> functionLinksPtr;

	vector<int> graphTraverseOrder;

	shared_ptr<FunctionLink> newFunctionLink;

	InteractionState interactionState;
	XMFLOAT2 clickOffset;

	void Validate(shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr, shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr);

	public:

	FunctionGraph();
	~FunctionGraph();

	DirectXTexturePtr GetBaseColorTexture();
	DirectXTexturePtr GetMetallicTexture();
	DirectXTexturePtr GetRoughnessTexture();
	DirectXTexturePtr GetNormalTexture();

	int GetSelectedNodeIndex();
	int GetSelectedNodeFunctionIndex();
	int GetSelectedNodeIntParameter(int parameterIndex);
	float GetSelectedNodeFloatParameter(int parameterIndex);

	void SetSelectedNodeIntParameter(int parameterIndex, int value);
	void SetSelectedNodeFloatParameter(int parameterIndex, float value);

	void AddNode(int functionIndex, float x, float y);
	void AddLink(int inputNodeIndex, int outputNodeIndex, int inputPinIndex);

	void SelectNode(int nodeIndex);
	void SelectLink(int linkIndex);

	void RemoveSelectedNode();
	void RemoveSelectedLink();

	void RemoveSelected();

	void OnMouseDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int x, int y);

	void Process();

	void Render();
};

