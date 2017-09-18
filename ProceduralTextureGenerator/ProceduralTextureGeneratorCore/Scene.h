#pragma once

#include "DirectXView.h"
#include "Camera.h"
#include "RenderableObject.h"
#include "Environment.h"


class Scene
{
	private:

	bool isInitialized;

	shared_ptr<DirectXView> view;

	shared_ptr<Camera> camera;
	shared_ptr<Environment> environment;

	map<string, shared_ptr<RenderableObject>> renderableObjects;

	void CheckInitialization();

	public:

	Scene();
	~Scene();

	void SetView(shared_ptr<DirectXView> view_);
	void SetCamera(shared_ptr<Camera> camera_);
	void SetEnvironment(shared_ptr<Environment> environment_);

	shared_ptr<DirectXView> GetView();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Environment> GetEnvironment();

	void AddRenderableObject(shared_ptr<RenderableObject> renderableObject, string name);

	void Render();
	void Release();
};

