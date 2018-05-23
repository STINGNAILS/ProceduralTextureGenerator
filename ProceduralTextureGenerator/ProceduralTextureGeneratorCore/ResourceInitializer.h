#pragma once

#include "DirectXObjectPool.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "Environment.h"
#include "TextureFrame.h"
#include "Port.h"
#include "Polyline.h"


class ResourceInitializer
{
	private:

	ResourceInitializer();
	~ResourceInitializer();

	static void InitializePolygonMeshes();
	static void InitializeVertexShaders();
	static void InitializePixelShaders();
	static void InitializeRasterizerStates();
	static void InitializeSamplerStates();
	static void InitializeConstantBuffers();

	public:

	static void InitializeRecources();
};

