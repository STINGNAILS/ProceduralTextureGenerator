// ProceduralTextureGeneratorCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DirectXDevice.h"
#include "DirectXView.h"
#include "ResourceInitializer.h"
#include "Scene.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Cube.h"
#include "FunctionGraph.h"
#include "TextureQuad.h"


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


map<int, shared_ptr<Scene>> scenes;

shared_ptr<FunctionGraph> functionGraph;

shared_ptr<Cube> cube1;

shared_ptr<TextureQuad> textureQuad;

string errorMessage;


CPP_API int Init()
{
	try
	{
		DirectXDevice::Init();
		ResourceInitializer::InitializeRecources();

		scenes[1] = make_shared<Scene>();
		scenes[2] = make_shared<Scene>();
		scenes[3] = make_shared<Scene>();

		shared_ptr<Camera3D> camera1 = make_shared<Camera3D>();
		camera1->SetYFOV(0.785398f);
		camera1->SetZNEAR(0.1f);
		camera1->SetZFAR(1000.0f);
		camera1->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
		camera1->SetRotation(XMFLOAT2(-1.57f, 3.14f));
		camera1->SetVelocity(5.0f);

		scenes[1]->SetCamera(camera1);


		shared_ptr<Environment> environment1 = make_shared<Environment>(L"Cubemap.dds");

		DirectionalLight dirLight1;
		dirLight1.intensity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		dirLight1.direction = XMFLOAT4(0.4f, -0.824621f, -0.4f, 0.0f);
		environment1->AddDirectionalLight(dirLight1);

		scenes[1]->SetEnvironment(environment1);


		shared_ptr<BackgroundGrid> backgroundGrid = make_shared<BackgroundGrid>();

		scenes[2]->SetBackground(backgroundGrid);


		functionGraph = make_shared<FunctionGraph>();

		scenes[2]->AddRenderableObject(functionGraph, "FunctionGraph");


		cube1 = make_shared<Cube>();
		cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
		cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
		cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
		cube1->SetNormalMap(functionGraph->GetNormalTexture());

		scenes[1]->AddRenderableObject(cube1, "Cube1");


		shared_ptr<Camera2D> camera2 = make_shared<Camera2D>();
		camera2->SetPosition(XMFLOAT2(0.0f, 0.0f));
		camera2->SetVelocity(5.0f);

		scenes[2]->SetCamera(camera2);


		shared_ptr<Camera2D> camera3 = make_shared<Camera2D>();
		camera3->SetPosition(XMFLOAT2(0.0f, 0.0f));
		camera3->SetVelocity(10.0f);

		scenes[3]->SetCamera(camera3);


		textureQuad = make_shared<TextureQuad>();
		textureQuad->SetTexture(functionGraph->GetTrackedTexture());

		scenes[3]->AddRenderableObject(textureQuad, "TextureQuad");
	}
	catch(string e)
	{
		errorMessage = e;
		return 1;
	}

	return 0;
}


CPP_API int ResizeView(int viewIndex, void *viewResource)
{
	try
	{
		shared_ptr<DirectXView> view = make_shared<DirectXView>(viewResource);

		scenes[viewIndex]->SetView(view);
		scenes[viewIndex]->GetCamera()->Resize(view->GetWidth(), view->GetHeight());
	}
	catch(string e)
	{
		errorMessage = e;
		return 1;
	}

	return 0;
}


CPP_API void Navigate(int viewIndex, float dx, float dy, float dt)
{
	scenes[viewIndex]->GetCamera()->Navigate(dx, dy, dt);
}


CPP_API void Zoom(int viewIndex, float x, float y, float dz)
{
	scenes[viewIndex]->GetCamera()->Zoom(x, y, dz);
}


CPP_API void Render(int viewIndex)
{
	scenes[viewIndex]->Render();
}


CPP_API void Release()
{
	cube1 = nullptr;
	textureQuad = nullptr;
	functionGraph = nullptr;

	scenes.clear();

	/*ID3D11Debug* debug = 0;
	device->GetDevice()->QueryInterface(IID_ID3D11Debug, (void**) &debug);*/
	DirectXDevice::Release();
	/*debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/
}


//CPP_API string GetErrorMessage()
//{
//	return errorMessage;
//}


CPP_API void GraphViewAddNode(int functionIndex, float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);

	functionGraph->AddNode(functionIndex, world.x, world.y);
}


CPP_API void GraphViewOnMouseDown(float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);

	functionGraph->OnMouseDown(world.x, world.y);
}


CPP_API void GraphViewOnMouseMove(float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);

	functionGraph->OnMouseMove(world.x, world.y);
}


CPP_API void GraphViewOnMouseUp(float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);
	
	functionGraph->OnMouseUp(world.x, world.y);

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());

	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
}


CPP_API void GraphViewOnMouseDoubleClick(float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);
	
	functionGraph->OnMouseDoubleClick(world.x, world.y);
	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
}


CPP_API void GraphViewRemoveSelected()
{
	functionGraph->RemoveSelected();

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());

	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
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

	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
}


CPP_API void ResetFunctionGraph()
{
	functionGraph = make_shared<FunctionGraph>();

	scenes[2]->AddRenderableObject(functionGraph, "FunctionGraph");

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());

	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
}


CPP_API void SaveFunctionGraphToFile(LPSTR fileName)
{
	functionGraph->SaveToFile(fileName);
}


CPP_API void LoadFunctionGraphFromFile(LPSTR fileName)
{
	functionGraph->LoadFromFile(fileName);

	cube1->SetBaseColorMap(functionGraph->GetBaseColorTexture());
	cube1->SetMetallicMap(functionGraph->GetMetallicTexture());
	cube1->SetRoughnessMap(functionGraph->GetRoughnessTexture());
	cube1->SetNormalMap(functionGraph->GetNormalTexture());

	textureQuad->SetTexture(functionGraph->GetTrackedTexture());
}