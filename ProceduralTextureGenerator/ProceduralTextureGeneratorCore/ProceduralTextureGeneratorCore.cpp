// ProceduralTextureGeneratorCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DirectXDevice.h"
#include "DirectXView.h"
#include "ResourceInitializer.h"
#include "Scene.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "DummyObject.h"
#include "FunctionGraph.h"
#include "TextureQuad.h"
#include "EnvironmentTaskQueue.h"


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


map<int, shared_ptr<Scene>> scenes;

shared_ptr<FunctionGraph> functionGraph;
shared_ptr<DummyObject> dummyObject;
CurrentDummyObject currentDummyObject;
shared_ptr<TextureQuad> textureQuad;

shared_ptr<EnvironmentTaskQueue> environmentTaskQueue;

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


		shared_ptr<Environment> environment1 = make_shared<Environment>();
		environment1->SetEnvironmentMap(L"Environment\\Citadella.dds", 0);

		DirectionalLight dirLight;
		dirLight.color = XMFLOAT3(1.0f, 1.0f, 1.0f);
		dirLight.intensity = 3.14f;
		dirLight.direction = XMFLOAT2(30.0f, 315.0f);
		dirLight.isEnabled = 1;
		environment1->SetDirectionalLight(dirLight);

		scenes[1]->SetEnvironment(environment1);


		shared_ptr<BackgroundGrid> backgroundGrid = make_shared<BackgroundGrid>();

		scenes[2]->SetBackground(backgroundGrid);


		functionGraph = make_shared<FunctionGraph>();

		scenes[2]->AddRenderableObject(functionGraph, "FunctionGraph");


		dummyObject = make_shared<DummyObject>("Cube");
		currentDummyObject = CUBE;

		functionGraph->BindDummyObject(dummyObject);

		scenes[1]->AddRenderableObject(dummyObject, "DummyObject");


		shared_ptr<Camera2D> camera2 = make_shared<Camera2D>();
		camera2->SetPosition(XMFLOAT2(0.0f, 0.0f));
		camera2->SetVelocity(5.0f);

		scenes[2]->SetCamera(camera2);


		shared_ptr<Camera2D> camera3 = make_shared<Camera2D>();
		camera3->SetPosition(XMFLOAT2(0.0f, 0.0f));
		camera3->SetVelocity(10.0f);

		scenes[3]->SetCamera(camera3);


		textureQuad = make_shared<TextureQuad>();

		functionGraph->BindTextureQuad(textureQuad);

		scenes[3]->AddRenderableObject(textureQuad, "TextureQuad");

		environmentTaskQueue = make_shared<EnvironmentTaskQueue>();
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


CPP_API void ProcessEnvironmentTasks()
{
	environmentTaskQueue->Process();
}


CPP_API void Release()
{
	dummyObject = nullptr;
	textureQuad = nullptr;
	functionGraph = nullptr;

	scenes.clear();

	DirectXObjectPool::Release();

	//ID3D11Debug* debug = 0;
	//DirectXDevice::GetDevice()->QueryInterface(IID_ID3D11Debug, (void**) &debug);
	DirectXDevice::Release();
	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}


//CPP_API string GetErrorMessage()
//{
//	return errorMessage;
//}


CPP_API int ObjectViewGetObjectType()
{
	return currentDummyObject;
}


CPP_API void ObjectViewSetObjectType(int objectType)
{
	switch(objectType)
	{
		case 0:
		{
			dummyObject = make_shared<DummyObject>("Cube");
			currentDummyObject = CUBE;
			break;
		}
		case 1:
		{
			dummyObject = make_shared<DummyObject>("Sphere");
			currentDummyObject = SPHERE;
			break;
		}
		case 2:
		{
			dummyObject = make_shared<DummyObject>("Plane");
			currentDummyObject = PLANE;
			break;
		}
	}

	functionGraph->BindDummyObject(dummyObject);

	scenes[1]->AddRenderableObject(dummyObject, "DummyObject");
}


CPP_API DirectionalLight ObjectViewGetDirectionalLight()
{
	return scenes[1]->GetEnvironment()->GetDirectionalLight();
}


CPP_API SphereLight ObjectViewGetSphereLight(int sphereLightIndex)
{
	return scenes[1]->GetEnvironment()->GetSphereLight(sphereLightIndex);
}


CPP_API int ObjectViewGetEnvironmentMapIndex()
{
	return scenes[1]->GetEnvironment()->GetEnvironmentMapIndex();
}


CPP_API void ObjectViewSetDirectionalLight(DirectionalLight directionalLight)
{
	scenes[1]->GetEnvironment()->SetDirectionalLight(directionalLight);
}


CPP_API void ObjectViewSetSphereLight(int sphereLightIndex, SphereLight sphereLight)
{
	scenes[1]->GetEnvironment()->SetSphereLight(sphereLightIndex, sphereLight);
}


CPP_API void ObjectViewSetEnvironmentMap(int environmentMapIndex)
{
	environmentTaskQueue->AddTask(scenes[1]->GetEnvironment(), environmentMapIndex);
}


CPP_API void ObjectViewScope()
{
	shared_ptr<Camera> camera = scenes[1]->GetCamera();
	Camera3D &camera1 = dynamic_cast<Camera3D&>(*camera.get());

	camera1.SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	camera1.SetRotation(XMFLOAT2(-1.57f, 3.14f));
}


CPP_API void GraphViewAddNode(int functionIndex, float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);

	functionGraph->AddNode(functionIndex, world.x, world.y);
}


CPP_API int GraphViewGraphWasChanged()
{
	return functionGraph->GraphWasChanged();
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
}


CPP_API void GraphViewOnMouseDoubleClick(float x, float y)
{
	XMFLOAT3 world = scenes[2]->GetCamera()->ScreenToWorld(x, y);
	
	functionGraph->OnMouseDoubleClick(world.x, world.y);
}


CPP_API void GraphViewRemoveSelected()
{
	functionGraph->RemoveSelected();
}


CPP_API HoveredPortDescriptor GraphViewGetHoveredPointDescriptor()
{
	HoveredPortDescriptor hoveredPortDescriptor = functionGraph->GetHoveredPortDescriptor();
	XMFLOAT3 screenCoords = scenes[2]->GetCamera()->WorldToScreen(hoveredPortDescriptor.position.x, hoveredPortDescriptor.position.y + 20.0f);

	hoveredPortDescriptor.position = XMFLOAT2(screenCoords.x, screenCoords.y);

	return hoveredPortDescriptor;
}


CPP_API float GraphViewGetScale()
{
	return scenes[2]->GetCamera()->GetScale();
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


CPP_API void GraphViewScope()
{
	XMFLOAT4 scope = functionGraph->GetScope();
	
	scenes[2]->GetCamera()->Scope(scope.x, scope.y, scope.z, scope.w);
}


CPP_API void GraphViewSaveMaterialTexturesToFiles(LPCWSTR baseColorFileName, LPCWSTR metallicFileName, LPCWSTR roughnessFileName, LPCWSTR normalFileName, int formatIndex)
{
	functionGraph->SaveMaterialTexturesToFiles(baseColorFileName, metallicFileName, roughnessFileName, normalFileName, formatIndex);
}


CPP_API void GraphViewProcess()
{
	functionGraph->Process();
}


CPP_API void GraphViewAbortProcessing()
{
	functionGraph->AbortProcessing();
}


CPP_API void TextureViewScope()
{
	scenes[3]->GetCamera()->Scope(-256.0f, -256.0f, 256.0f, 256.0f);
}


CPP_API void TextureViewSaveTrackedTextureToFile(LPCWSTR fileName, int formatIndex)
{
	functionGraph->SaveTrackedTextureToFile(fileName, formatIndex);
}


CPP_API void ResetFunctionGraph()
{
	functionGraph->Reset();
}


CPP_API void SaveFunctionGraphToFile(LPSTR fileName)
{
	functionGraph->SaveToFile(fileName);
}


CPP_API void LoadFunctionGraphFromFile(LPSTR fileName)
{
	functionGraph->LoadFromFile(fileName);
}