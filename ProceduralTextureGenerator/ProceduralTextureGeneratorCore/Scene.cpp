#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
	view = nullptr;
	camera = nullptr;
	environment = nullptr;

	isInitialized = false;
}


Scene::~Scene()
{
	Release();
}


void Scene::CheckInitialization()
{
	if(view.get() != nullptr && camera.get() != nullptr)
	{
		isInitialized = true;
	}
}


void Scene::SetView(shared_ptr<DirectXView> view_)
{
	view = view_;
	CheckInitialization();
}


void Scene::SetCamera(shared_ptr<Camera> camera_)
{
	camera = camera_;
	CheckInitialization();
}


void Scene::SetEnvironment(shared_ptr<Environment> environment_)
{
	environment = environment_;
}


shared_ptr<DirectXView> Scene::GetView()
{
	return view;
}


shared_ptr<Camera> Scene::GetCamera()
{
	return camera;
}


shared_ptr<Environment> Scene::GetEnvironment()
{
	return environment;
}


shared_ptr<RenderableObject> Scene::GetRenderableObject(string name)
{
	return renderableObjects[name];
}


void Scene::AddRenderableObject(shared_ptr<RenderableObject> renderableObject, string name)
{
	renderableObjects[name] = renderableObject;
}


void Scene::Render()
{
	if(isInitialized)
	{
		view->BeginRender();

		camera->Prepare();
		if(environment) environment->Prepare();

		for(auto it = renderableObjects.begin(); it != renderableObjects.end(); it++)
		{
			it->second->Render();
		}

		if(environment) environment->Render();

		view->FinishRender();
	}
}


void Scene::Release()
{
	view = nullptr;
	camera = nullptr;
	environment = nullptr;

	renderableObjects.clear();
}
