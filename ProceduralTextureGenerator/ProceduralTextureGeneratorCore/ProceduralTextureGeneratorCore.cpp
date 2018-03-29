// ProceduralTextureGeneratorCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DirectXDevice.h"
#include "DirectXView.h"
#include "Scene.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Cube.h"
#include "FunctionGraph.h"


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


map<int, shared_ptr<Scene>> scenes;

shared_ptr<FunctionGraph> functionGraph;

shared_ptr<Cube> cube1;


CPP_API void Init()
{
	DirectXDevice::Init();

	scenes[1] = make_shared<Scene>();
	scenes[2] = make_shared<Scene>();
	scenes[3] = make_shared<Scene>();

	shared_ptr<Camera3D> camera1 = make_shared<Camera3D>();
	camera1->Init();
	camera1->SetYFOV(0.785398f);
	camera1->SetZNEAR(0.1f);
	camera1->SetZFAR(1000.0f);
	camera1->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	camera1->SetRotation(XMFLOAT2(-1.57f, 3.14f));
	camera1->SetVelocity(5.0f);

	scenes[1]->SetCamera(camera1);


	shared_ptr<Environment> environment1 = make_shared<Environment>();
	environment1->Init();
	environment1->InitEnvironment(L"Cubemap.dds");

	DirectionalLight dirLight1;
	dirLight1.intensity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight1.direction = XMFLOAT4(0.4f, -0.824621f, -0.4f, 0.0f);
	environment1->AddDirectionalLight(dirLight1);

	scenes[1]->SetEnvironment(environment1);


	functionGraph = make_shared<FunctionGraph>();

	scenes[2]->AddRenderableObject(functionGraph, "FunctionGraph");


	cube1 = make_shared<Cube>();
	cube1->Init();
	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());

	scenes[1]->AddRenderableObject(cube1, "Cube1");


	shared_ptr<Camera2D> camera2 = make_shared<Camera2D>();
	camera2->Init();
	camera2->SetPosition(XMFLOAT2(0.0f, 0.0f));
	camera2->SetVelocity(5.0f);

	scenes[2]->SetCamera(camera2);
}


CPP_API void BindView(int viewIndex)
{
	shared_ptr<DirectXView> view = make_shared<DirectXView>();

	scenes[viewIndex]->SetView(view);
}


CPP_API void OverrideView(int viewIndex, void *viewResource)
{
	shared_ptr<DirectXView> view = scenes[viewIndex]->GetView();
	shared_ptr<Camera> camera = scenes[viewIndex]->GetCamera();

	view->Init(viewResource);
	camera->Resize(view->GetWidth(), view->GetHeight());
}


CPP_API void Navigate(int viewIndex, float dx, float dy, float dt)
{
	scenes[viewIndex]->GetCamera()->Navigate(dx, dy, dt);
}


CPP_API void Zoom(int viewIndex, float dz)
{
	scenes[viewIndex]->GetCamera()->Zoom(dz);
}


CPP_API void Render(int viewIndex)
{
	scenes[viewIndex]->Render();
}


CPP_API void Release()
{
	scenes.clear();

	/*ID3D11Debug* debug = 0;
	device->GetDevice()->QueryInterface(IID_ID3D11Debug, (void**) &debug);*/
	DirectXDevice::Release();
	/*debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/
}


CPP_API void GraphViewAddNode(int functionIndex, float x, float y)
{
	XMFLOAT3 position = scenes[2]->GetCamera()->Position();
	XMFLOAT2 widthHeight = scenes[2]->GetCamera()->WidthHeight();
	float zoom = scenes[2]->GetCamera()->Zoom();

	functionGraph->AddNode(functionIndex, (position.x + widthHeight.x * (x - 0.5f)) / zoom, (position.y + widthHeight.y * (y - 0.5f)) / zoom);
	functionGraph->Process();
}


CPP_API void GraphViewOnMouseDown(float x, float y)
{
	XMFLOAT3 position = scenes[2]->GetCamera()->Position();
	XMFLOAT2 widthHeight = scenes[2]->GetCamera()->WidthHeight();
	float zoom = scenes[2]->GetCamera()->Zoom();

	functionGraph->OnMouseDown((position.x + widthHeight.x * (x - 0.5f)) / zoom, (position.y + widthHeight.y * (y - 0.5f)) / zoom);
}


CPP_API void GraphViewOnMouseMove(float x, float y)
{
	XMFLOAT3 position = scenes[2]->GetCamera()->Position();
	XMFLOAT2 widthHeight = scenes[2]->GetCamera()->WidthHeight();
	float zoom = scenes[2]->GetCamera()->Zoom();

	functionGraph->OnMouseMove((position.x + widthHeight.x * (x - 0.5f)) / zoom, (position.y + widthHeight.y * (y - 0.5f)) / zoom);
}


CPP_API void GraphViewOnMouseUp(float x, float y)
{
	XMFLOAT3 position = scenes[2]->GetCamera()->Position();
	XMFLOAT2 widthHeight = scenes[2]->GetCamera()->WidthHeight();
	float zoom = scenes[2]->GetCamera()->Zoom();
	
	functionGraph->OnMouseUp((position.x + widthHeight.x * (x - 0.5f)) / zoom, (position.y + widthHeight.y * (y - 0.5f)) / zoom);

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());
}


CPP_API void GraphViewRemoveSelected()
{
	functionGraph->RemoveSelected();

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());
}


CPP_API int GraphViewGetSelectedNodeIndex()
{
	return functionGraph->GetSelectedNodeIndex();
}


CPP_API int GraphViewGetSelectedNodeFunctionIndex()
{
	return functionGraph->GetSelectedNodeFunctionIndex();
}


CPP_API int GraphViewGetSelectedNodeIntParameter(int parameterIndex)
{
	return functionGraph->GetSelectedNodeIntParameter(parameterIndex);
}


CPP_API float GraphViewGetSelectedNodeFloatParameter(int parameterIndex)
{
	return functionGraph->GetSelectedNodeFloatParameter(parameterIndex);
}


CPP_API void GraphViewSetSelectedNodeIntParameter(int parameterIndex, int value)
{
	functionGraph->SetSelectedNodeIntParameter(parameterIndex, value);
}


CPP_API void GraphViewSetSelectedNodeFloatParameter(int parameterIndex, float value)
{
	functionGraph->SetSelectedNodeFloatParameter(parameterIndex, value);
}


CPP_API void GraphViewProcess()
{
	functionGraph->Process();

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());
}