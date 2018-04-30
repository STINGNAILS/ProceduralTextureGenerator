#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
	view = nullptr;
	camera = nullptr;
	backgroundGrid = nullptr;
	environment = nullptr;

	isInitialized = false;
}


Scene::~Scene()
{
	
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


void Scene::SetBackground(shared_ptr<BackgroundGrid> background_)
{
	backgroundGrid = background_;
}


void Scene::SetEnvironment(shared_ptr<Environment> environment_)
{
	environment = environment_;
}


void Scene::AddRenderableObject(shared_ptr<RenderableObject> renderableObject, string name)
{
	renderableObjects[name] = renderableObject;
}


shared_ptr<DirectXView> Scene::GetView()
{
	return view;
}


shared_ptr<Camera> Scene::GetCamera()
{
	return camera;
}


shared_ptr<BackgroundGrid> Scene::GetBackground()
{
	return backgroundGrid;
}


shared_ptr<Environment> Scene::GetEnvironment()
{
	return environment;
}


shared_ptr<RenderableObject> Scene::GetRenderableObject(string name)
{
	return renderableObjects[name];
}


void Scene::Render()
{
	if(isInitialized)
	{
		view->BeginRender();

		camera->Set();

		if(backgroundGrid)
		{
			backgroundGrid->Render();
		}

		if(environment)
		{
			environment->Render();
			environment->Set();
		}

		for(auto it = renderableObjects.begin(); it != renderableObjects.end(); it++)
		{
			it->second->Render();
		}


		view->FinishRender();
	}
}