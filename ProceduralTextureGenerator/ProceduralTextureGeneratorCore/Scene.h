#pragma once

#include "DirectXView.h"
#include "Camera.h"
#include "RenderableObject.h"
#include "Environment.h"
#include "BackgroundGrid.h"


class Scene
{
	private:

	bool isInitialized;

	shared_ptr<DirectXView> view;

	shared_ptr<Camera> camera;

	shared_ptr<BackgroundGrid> backgroundGrid;
	shared_ptr<Environment> environment;

	map<string, shared_ptr<RenderableObject>> renderableObjects;

	void CheckInitialization();

	public:

	Scene();
	~Scene();

	void SetView(shared_ptr<DirectXView> view_);
	void SetCamera(shared_ptr<Camera> camera_);
	void SetBackground(shared_ptr<BackgroundGrid> background_);
	void SetEnvironment(shared_ptr<Environment> environment_);
	void AddRenderableObject(shared_ptr<RenderableObject> renderableObject, string name);

	shared_ptr<DirectXView> GetView();
	shared_ptr<Camera> GetCamera();
	shared_ptr<BackgroundGrid> GetBackground();
	shared_ptr<Environment> GetEnvironment();
	shared_ptr<RenderableObject> GetRenderableObject(string name);

	void Render();
};

