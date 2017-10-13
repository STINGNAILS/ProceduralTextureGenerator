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


	shared_ptr<Environment> environment1 = make_shared<Environment>(device);

	DirectionalLight dirLight1;
	dirLight1.intensity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight1.direction = XMFLOAT4(0.4f, -0.824621f, -0.4f, 0.0f);
	environment1->AddDirectionalLight(dirLight1);

	environment1->Init(L"Cubemap.dds");

	scenes[1]->SetEnvironment(environment1);


	shared_ptr<Cube> cube1 = make_shared<Cube>(device);
	cube1->Init();

	shared_ptr<TextureMemory> baseColorMapPtr;
	shared_ptr<TextureMemory> metallicMapPtr;
	shared_ptr<TextureMemory> roughnessMapPtr;
	shared_ptr<TextureMemory> normalMapPtr;

	mapGenerator.GenerateRustyIronMaps(2048, baseColorMapPtr, metallicMapPtr, roughnessMapPtr);
	normalMapPtr = mapGenerator.GenerateNormalMap(2048, XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f));

	shared_ptr<DirectXTexture> baseColorMapTexture = make_shared<DirectXTexture>(device);
	shared_ptr<DirectXTexture> metallicMapTexture = make_shared<DirectXTexture>(device);
	shared_ptr<DirectXTexture> roughnessMapTexture = make_shared<DirectXTexture>(device);
	shared_ptr<DirectXTexture> normalMapTexture = make_shared<DirectXTexture>(device);

	baseColorMapTexture->InitFromMemory(baseColorMapPtr, BPC16);
	metallicMapTexture->InitFromMemory(metallicMapPtr, BPC16);
	roughnessMapTexture->InitFromMemory(roughnessMapPtr, BPC16);
	normalMapTexture->InitFromMemory(normalMapPtr, BPC16);

	cube1->SetBaseColorMap(baseColorMapTexture);
	cube1->SetMetallicMap(metallicMapTexture);
	cube1->SetRoughnessMap(roughnessMapTexture);
	cube1->SetNormalMap(normalMapTexture);

	scenes[1]->AddRenderableObject(cube1, "Cube1");


	/*shared_ptr<Camera> camera2(new Camera());
	camera2->InitOrthographic(device);
	camera2->SetTransform(XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT2(-1.57f, 3.14f));
	camera2->SetVelocity(5.0f);*/

	//scenes[2]->SetCamera(camera1);
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

	view->Init(device, viewResource);
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

	//ID3D11Debug* debug = 0;
	//device->GetDevice()->QueryInterface(IID_ID3D11Debug, (void**) &debug);
	device = nullptr;
	//debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}