// ProceduralTextureGeneratorCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DirectXDevice.h"
#include "DirectXView.h"
#include "Scene.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Cube.h"
#include "MapGenerator.h"


using namespace std;


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


shared_ptr<DirectXDevice> device;

map<int, shared_ptr<DirectXView>> views;
map<int, shared_ptr<Camera>> cameras;
map<int, shared_ptr<Scene>> scenes;

MapGenerator mapGenerator;


CPP_API void Init()
{
	if(device.get() != nullptr)
	{
		return;
	}

	device = make_shared<DirectXDevice>();
	device->Init();

	scenes[1] = make_shared<Scene>();
	//scenes[2] = make_shared<Scene>();
	//scenes[3] = make_shared<Scene>();


	shared_ptr<Camera3D> camera1 = make_shared<Camera3D>();
	camera1->Init(device);
	camera1->SetYFOV(0.785398f);
	camera1->SetZNEAR(0.1f);
	camera1->SetZFAR(1000.0f);
	camera1->SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	camera1->SetRotation(XMFLOAT2(-1.57f, 3.14f));
	camera1->SetVelocity(5.0f);

	scenes[1]->SetCamera(camera1);


	shared_ptr<Environment> environment1 = make_shared<Environment>();

	DirectionalLight dirLight1;
	dirLight1.intensity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight1.direction = XMFLOAT4(0.4f, -0.824621f, -0.4f, 0.0f);
	environment1->AddDirectionalLight(dirLight1);

	environment1->Init(device, L"Cubemap.dds");

	scenes[1]->SetEnvironment(environment1);


	shared_ptr<Cube> cube1(new Cube());
	cube1->Init(device);

	vector<float> baseColorMap;
	vector<float> metallicMap;
	vector<float> roughnessMap;
	vector<float> normalMap;

	mapGenerator.GenerateRustyIronMaps(1024, baseColorMap, metallicMap, roughnessMap);
	normalMap = mapGenerator.GenerateNormalMap(1024, XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f));

	cube1->SetBaseColorMap(baseColorMap);
	cube1->SetMetallicMap(metallicMap);
	cube1->SetRoughnessMap(roughnessMap);
	cube1->SetNormalMap(normalMap);

	scenes[1]->AddRenderableObject(cube1, "Cube1");


	/*shared_ptr<Camera> camera2(new Camera());
	camera2->InitOrthographic(device);
	camera2->SetTransform(XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT2(-1.57f, 3.14f));
	camera2->SetVelocity(5.0f);*/

	//scenes[2]->SetCamera(camera1);
}


CPP_API void BindView(int viewIndex, int hwnd)
{
	views[viewIndex] = make_shared<DirectXView>();
	views[viewIndex]->Init((HWND)hwnd, device);

	scenes[viewIndex]->SetView(views[viewIndex]);
}


CPP_API void ResizeView(int viewIndex, int hwnd)
{
	views[viewIndex]->Resize((HWND)hwnd);

	RECT rc;
	GetClientRect((HWND) hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	scenes[viewIndex]->GetCamera()->Resize(width, height);
}


CPP_API void Navigate(int viewIndex, float dx, float dy, float dt)
{
	scenes[viewIndex]->GetCamera()->Navigate(dx, dy, dt);
}


CPP_API void Zoom(int viewIndex, float dz)
{
	scenes[viewIndex]->GetCamera()->Zoom(dz);
}


CPP_API void Render()
{
	for(auto it = scenes.begin(); it != scenes.end(); it++)
	{
		it->second->Render();
	}
}


CPP_API void Release()
{
	for(auto it = views.begin(); it != views.end(); it++)
	{
		it->second->Release();
	}

	device->Release();
}