#pragma once

#include "RenderableObject.h"
#include "FunctionNode.h"
#include "Function.h"
#include "DummyObject.h"
#include "TextureQuad.h"


struct HoveredPortDescriptor
{
	int hoveredPortFunctionIndex;
	int hoveredPortIndex;
	XMFLOAT2 position;
};


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
	int trackedNodeIndex;
	shared_ptr<map<int, FunctionNode>> functionNodesPtr;

	int nextLinkIndex;
	int selectedLinkIndex;
	shared_ptr<map<int, FunctionLink>> functionLinksPtr;

	bool graphWasChanged;

	shared_ptr<vector<int>> graphTraverseOrderPtr;

	shared_ptr<FunctionLink> newFunctionLink;

	mutex functionGraphMutex;

	bool abortProcessing;

	shared_ptr<DummyObject> dummyObjectPtr;
	shared_ptr<TextureQuad> textureQuadPtr;

	InteractionState interactionState;
	XMFLOAT2 clickOffset;

	HoveredPortDescriptor hoveredPortDescriptor;

	void Validate(shared_ptr<map<int, FunctionNode>> functionNodesCopyPtr, shared_ptr<map<int, FunctionLink>> functionLinksCopyPtr);

	void UpdateDummyObjectTexture();
	void UpdateTextureQuadTexture();

	public:

	FunctionGraph();
	~FunctionGraph();

	void BindDummyObject(shared_ptr<DummyObject> dummyObjectPtr_);
	void BindTextureQuad(shared_ptr<TextureQuad> textureQuadPtr_);

	int GetSelectedNodeIndex();
	int GetSelectedNodeFunctionIndex();
	int GetSelectedNodeIntParameter(int parameterIndex);
	float GetSelectedNodeFloatParameter(int parameterIndex);
	XMFLOAT4 GetScope();
	HoveredPortDescriptor GetHoveredPortDescriptor();

	void SetSelectedNodeIntParameter(int parameterIndex, int value);
	void SetSelectedNodeFloatParameter(int parameterIndex, float value);

	bool GraphWasChanged();

	void AddNode(int functionIndex, float x, float y);
	void AddLink(int inputNodeIndex, int outputNodeIndex, int inputPortIndex);

	void SelectNode(int nodeIndex);
	void SelectLink(int linkIndex);

	void RemoveSelectedNode();
	void RemoveSelectedLink();

	void RemoveSelected();

	void OnMouseDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseDoubleClick(int x, int y);

	void Process();
	void AbortProcessing();

	void Render();

	void Reset();

	void SaveToFile(LPSTR fileName);
	void LoadFromFile(LPSTR fileName);
	
	void SaveMaterialTexturesToFiles(LPCWSTR baseColorFileName, LPCWSTR metallicFileName, LPCWSTR roughnessFileName, LPCWSTR normalFileName, int formatIndex);
	void SaveTrackedTextureToFile(LPCWSTR fileName, int formatIndex);
};

