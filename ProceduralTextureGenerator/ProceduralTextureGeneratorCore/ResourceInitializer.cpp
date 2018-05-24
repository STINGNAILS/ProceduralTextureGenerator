#include "stdafx.h"
#include "ResourceInitializer.h"


struct SimpleVertex
{
	XMFLOAT3 pos;
};


struct BasicVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};


struct PBSVertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT2 tex;
};


struct PortVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 colorColor;
	XMFLOAT4 colorGrayscale;
	XMFLOAT4 colorMandatoryNotLinked;
	XMFLOAT4 colorNotMandatoryOrLinked;
	XMFLOAT2 uv;
};


struct LineVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT2 uv;
};


ResourceInitializer::ResourceInitializer()
{

}


ResourceInitializer::~ResourceInitializer()
{

}


void ResourceInitializer::InitializePolygonMeshes()
{
	vector<SimpleVertex> backgroundGridVertices(4);

	backgroundGridVertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f) };
	backgroundGridVertices[1] = { XMFLOAT3(1.0f, -1.0f, 0.0f) };
	backgroundGridVertices[2] = { XMFLOAT3(-1.0f, 1.0f, 0.0f) };
	backgroundGridVertices[3] = { XMFLOAT3(1.0f, 1.0f, 0.0f) };

	vector<UINT> backgroundGridIndices(6);

	backgroundGridIndices[0] = 0;
	backgroundGridIndices[1] = 2;
	backgroundGridIndices[2] = 3;
	backgroundGridIndices[3] = 0;
	backgroundGridIndices[4] = 3;
	backgroundGridIndices[5] = 1;

	DirectXObjectPool::SetPolygonMesh("BackgroundGrid", make_shared<PolygonMesh>((void*) &backgroundGridVertices[0], sizeof(SimpleVertex), backgroundGridVertices.size(), &backgroundGridIndices[0], backgroundGridIndices.size()));


	vector<SimpleVertex> cubeMapVertices(8);

	cubeMapVertices[0].pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	cubeMapVertices[1].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
	cubeMapVertices[2].pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	cubeMapVertices[3].pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
	cubeMapVertices[4].pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	cubeMapVertices[5].pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	cubeMapVertices[6].pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	cubeMapVertices[7].pos = XMFLOAT3(1.0f, -1.0f, -1.0f);

	vector<UINT> cubeMapIndices(36);

	cubeMapIndices[0] = 0;
	cubeMapIndices[1] = 1;
	cubeMapIndices[2] = 2;

	cubeMapIndices[3] = 1;
	cubeMapIndices[4] = 3;
	cubeMapIndices[5] = 2;

	cubeMapIndices[6] = 1;
	cubeMapIndices[7] = 5;
	cubeMapIndices[8] = 3;

	cubeMapIndices[9] = 5;
	cubeMapIndices[10] = 7;
	cubeMapIndices[11] = 3;

	cubeMapIndices[12] = 5;
	cubeMapIndices[13] = 4;
	cubeMapIndices[14] = 7;

	cubeMapIndices[15] = 4;
	cubeMapIndices[16] = 6;
	cubeMapIndices[17] = 7;

	cubeMapIndices[18] = 4;
	cubeMapIndices[19] = 0;
	cubeMapIndices[20] = 6;

	cubeMapIndices[21] = 0;
	cubeMapIndices[22] = 2;
	cubeMapIndices[23] = 6;

	cubeMapIndices[24] = 4;
	cubeMapIndices[25] = 5;
	cubeMapIndices[26] = 0;

	cubeMapIndices[27] = 5;
	cubeMapIndices[28] = 1;
	cubeMapIndices[29] = 0;

	cubeMapIndices[30] = 2;
	cubeMapIndices[31] = 3;
	cubeMapIndices[32] = 6;

	cubeMapIndices[33] = 3;
	cubeMapIndices[34] = 7;
	cubeMapIndices[35] = 6;

	DirectXObjectPool::SetPolygonMesh("Environment", make_shared<PolygonMesh>((void*) &cubeMapVertices[0], sizeof(SimpleVertex), cubeMapVertices.size(), &cubeMapIndices[0], cubeMapIndices.size()));


	vector<BasicVertex> textureRendererVertices(4);

	textureRendererVertices[0] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	textureRendererVertices[1] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	textureRendererVertices[2] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	textureRendererVertices[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

	vector<UINT> textureRendererIndices(6);

	textureRendererIndices[0] = 0;
	textureRendererIndices[1] = 1;
	textureRendererIndices[2] = 2;
	textureRendererIndices[3] = 1;
	textureRendererIndices[4] = 3;
	textureRendererIndices[5] = 2;

	DirectXObjectPool::SetPolygonMesh("TextureRenderer", make_shared<PolygonMesh>((void*) &textureRendererVertices[0], sizeof(BasicVertex), textureRendererVertices.size(), &textureRendererIndices[0], textureRendererIndices.size()));


	vector<PBSVertex> cubeVertices(24);

	XMFLOAT3 v1;
	XMFLOAT3 v2;
	XMFLOAT3 v3;
	XMFLOAT3 v4;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;

	v1 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	v2 = XMFLOAT3(1.0f, 1.0f, 1.0f);
	v3 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	v4 = XMFLOAT3(1.0f, -1.0f, 1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[0] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[1] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[2] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[3] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	v1 = XMFLOAT3(1.0f, 1.0f, 1.0f);
	v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v3 = XMFLOAT3(1.0f, -1.0f, 1.0f);
	v4 = XMFLOAT3(1.0f, -1.0f, -1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[4] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[5] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[6] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[7] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	v1 = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v2 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v3 = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v4 = XMFLOAT3(-1.0f, -1.0f, -1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[8] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[9] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[10] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[11] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v2 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v4 = XMFLOAT3(-1.0f, -1.0f, 1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[12] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[13] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[14] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[15] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	v1 = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v2 = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v3 = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	v4 = XMFLOAT3(1.0f, 1.0f, 1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[16] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[17] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[18] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[19] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	v1 = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	v2 = XMFLOAT3(1.0f, -1.0f, 1.0f);
	v3 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v4 = XMFLOAT3(1.0f, -1.0f, -1.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	cubeVertices[20] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	cubeVertices[21] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	cubeVertices[22] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	cubeVertices[23] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	vector<UINT> cubeIndices(36);

	for(int i = 0; i < 6; i++)
	{
		cubeIndices[6 * i + 0] = 4 * i + 0;
		cubeIndices[6 * i + 1] = 4 * i + 1;
		cubeIndices[6 * i + 2] = 4 * i + 2;
		cubeIndices[6 * i + 3] = 4 * i + 1;
		cubeIndices[6 * i + 4] = 4 * i + 3;
		cubeIndices[6 * i + 5] = 4 * i + 2;
	}

	DirectXObjectPool::SetPolygonMesh("Cube", make_shared<PolygonMesh>((void*) &cubeVertices[0], sizeof(PBSVertex), cubeVertices.size(), &cubeIndices[0], cubeIndices.size()));


	vector<PBSVertex> sphereVertices(3109);

	for(int j = 0; j < 60; j++)
	{
		float psi = (j + 0.5f) * 2 * _Pi / 60.0;

		float u = 0.5f + atan2(-sin(psi), -cos(psi)) / (2 * _Pi);

		XMFLOAT3 coords0 = XMFLOAT3(0.0f, 1.0f, 0.0f);
		XMFLOAT3 coords1 = XMFLOAT3(0.0f, -1.0f, 0.0f);
		XMFLOAT3 tangent = XMFLOAT3(-sin(psi), 0, -cos(psi));
		XMFLOAT2 uv0 = XMFLOAT2(u, 0.0f);
		XMFLOAT2 uv1 = XMFLOAT2(u, 1.0f);

		sphereVertices[j] = { coords0, coords0, tangent, uv0 };
		sphereVertices[j + 3049] = { coords1, coords1, tangent, uv1 };
	}

	for(int i = 1; i < 50; i++)
	{
		float phi = i * _Pi / 50.0;
		float y = cos(phi);

		float v = 0.5f - asin(y) / _Pi;

		float sinPhi = sin(phi);

		for(int j = 0; j < 60; j++)
		{
			float psi = j * 2 * _Pi / 60.0;
			float x = sinPhi * cos(psi);
			float z = -sinPhi * sin(psi);

			float u = 0.5f + atan2(z, -x) / (2 * _Pi);

			XMFLOAT3 coords = XMFLOAT3(x, y, z);
			XMFLOAT3 tangent = XMFLOAT3(-sin(psi), 0, -cos(psi));
			XMFLOAT2 uv = XMFLOAT2(u, v);

			sphereVertices[i * 61 + j - 1] = { coords, coords, tangent, uv };
		}

		XMFLOAT3 coords = XMFLOAT3(sinPhi, y, 0.0f);
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 0, -1.0f);
		XMFLOAT2 uv = XMFLOAT2(1.0f, v);

		sphereVertices[i * 61 + 59] = { coords, coords, tangent, uv };
	}

	vector<UINT> sphereIndices(17640);

	for(int j = 0; j < 60; j++)
	{
		sphereIndices[j * 3 + 0] = j;
		sphereIndices[j * 3 + 1] = j + 61;
		sphereIndices[j * 3 + 2] = j + 60;

		sphereIndices[j * 3 + 17460] = j + 3049;
		sphereIndices[j * 3 + 17461] = j + 2988;
		sphereIndices[j * 3 + 17462] = j + 2989;
	}

	for(int i = 1; i < 49; i++)
	{
		for(int j = 0; j < 60; j++)
		{
			sphereIndices[i * 360 + j * 6 - 180] = i * 61 + j + 60;
			sphereIndices[i * 360 + j * 6 - 179] = i * 61 + j - 1;
			sphereIndices[i * 360 + j * 6 - 178] = i * 61 + j;

			sphereIndices[i * 360 + j * 6 - 177] = i * 61 + j + 60;
			sphereIndices[i * 360 + j * 6 - 176] = i * 61 + j;
			sphereIndices[i * 360 + j * 6 - 175] = i * 61 + j + 61;
		}
	}

	DirectXObjectPool::SetPolygonMesh("Sphere", make_shared<PolygonMesh>((void*) &sphereVertices[0], sizeof(PBSVertex), sphereVertices.size(), &sphereIndices[0], sphereIndices.size()));


	vector<PBSVertex> planeVertices(4);

	v1 = XMFLOAT3(-2.0f, 0.0f, -2.0f);
	v2 = XMFLOAT3(2.0f, 0.0f, -2.0f);
	v3 = XMFLOAT3(-2.0f, 0.0f, 2.0f);
	v4 = XMFLOAT3(2.0f, 0.0f, 2.0f);

	normal = CalculateNormal(v1, v2, v3);
	tangent = CalculateTangent(v1, v2, v3, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	planeVertices[0] = { v1, normal, tangent, XMFLOAT2(0.0f, 0.0f) };
	planeVertices[1] = { v2, normal, tangent, XMFLOAT2(1.0f, 0.0f) };
	planeVertices[2] = { v3, normal, tangent, XMFLOAT2(0.0f, 1.0f) };
	planeVertices[3] = { v4, normal, tangent, XMFLOAT2(1.0f, 1.0f) };

	vector<UINT> planeIndices(6);

	planeIndices[0] = 0;
	planeIndices[1] = 1;
	planeIndices[2] = 2;
	planeIndices[3] = 1;
	planeIndices[4] = 3;
	planeIndices[5] = 2;

	DirectXObjectPool::SetPolygonMesh("Plane", make_shared<PolygonMesh>((void*) &planeVertices[0], sizeof(PBSVertex), planeVertices.size(), &planeIndices[0], planeIndices.size()));


	vector<BasicVertex> textureQuadVertices(4);

	textureQuadVertices[0] = { XMFLOAT3(-256.0f, -256.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	textureQuadVertices[1] = { XMFLOAT3(256.0f, -256.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	textureQuadVertices[2] = { XMFLOAT3(-256.0f, 256.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	textureQuadVertices[3] = { XMFLOAT3(256.0f, 256.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

	vector<UINT> textureQuadIndices(6);

	textureQuadIndices[0] = 0;
	textureQuadIndices[1] = 2;
	textureQuadIndices[2] = 3;
	textureQuadIndices[3] = 0;
	textureQuadIndices[4] = 3;
	textureQuadIndices[5] = 1;

	DirectXObjectPool::SetPolygonMesh("TextureQuad", make_shared<PolygonMesh>((void*) &textureQuadVertices[0], sizeof(BasicVertex), textureQuadVertices.size(), &textureQuadIndices[0], textureQuadIndices.size()));


	vector<BasicVertex> textureFrameVertices(4);

	textureFrameVertices[0] = { XMFLOAT3(-31.0f, -31.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	textureFrameVertices[1] = { XMFLOAT3(31.0f, -31.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	textureFrameVertices[2] = { XMFLOAT3(-31.0f, 31.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	textureFrameVertices[3] = { XMFLOAT3(31.0f, 31.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

	vector<UINT> textureFrameIndices(6);

	textureFrameIndices[0] = 0;
	textureFrameIndices[1] = 2;
	textureFrameIndices[2] = 3;
	textureFrameIndices[3] = 0;
	textureFrameIndices[4] = 3;
	textureFrameIndices[5] = 1;

	DirectXObjectPool::SetPolygonMesh("TextureFrame", make_shared<PolygonMesh>((void*) &textureFrameVertices[0], sizeof(BasicVertex), textureFrameVertices.size(), &textureFrameIndices[0], textureFrameIndices.size()));


	vector<PortVertex> portVertices(13);

	XMFLOAT3 pos1 = XMFLOAT3(-Port::r0, Port::r0, 0.0f);
	XMFLOAT3 pos2 = XMFLOAT3(Port::r0, Port::r0, 0.0f);
	XMFLOAT3 pos3 = XMFLOAT3(-Port::r0, -Port::r0, 0.0f);
	XMFLOAT3 pos4 = XMFLOAT3(Port::r0, -Port::r0, 0.0f);
	XMFLOAT3 pos5 = XMFLOAT3(-Port::r0, 0.577350f * Port::r0, 0.0f);
	XMFLOAT3 pos6 = XMFLOAT3(Port::r0, 0.577350f * Port::r0, 0.0f);
	XMFLOAT3 pos7 = XMFLOAT3(0.0f, -Port::r0, 0.0f);
	XMFLOAT3 pos8 = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT4 red = XMFLOAT4(0.9f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 0.9f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f);
	XMFLOAT4 lightGray = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	XMFLOAT4 gray = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 darkGray = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	XMFLOAT4 dark = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	XMFLOAT4 silver = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);

	XMFLOAT2 uv1 = XMFLOAT2(-1.0f, 1.0f);
	XMFLOAT2 uv2 = XMFLOAT2(1.0f, 1.0f);
	XMFLOAT2 uv3 = XMFLOAT2(-1.0f, -1.0f);
	XMFLOAT2 uv4 = XMFLOAT2(1.0f, -1.0f);
	XMFLOAT2 uv5 = XMFLOAT2(-1.0f, 0.577350f);
	XMFLOAT2 uv6 = XMFLOAT2(1.0f, 0.577350f);
	XMFLOAT2 uv7 = XMFLOAT2(0.0f, -1.0f);
	XMFLOAT2 uv8 = XMFLOAT2(0.0f, 0.0f);

	portVertices[0] = { pos1, red, lightGray, dark, silver, uv1 };
	portVertices[1] = { pos2, red, lightGray, dark, silver, uv2 };
	portVertices[2] = { pos5, red, lightGray, dark, silver, uv5 };
	portVertices[3] = { pos6, red, lightGray, dark, silver, uv6 };
	portVertices[4] = { pos8, red, lightGray, dark, silver, uv8 };

	portVertices[5] = { pos3, green, gray, dark, silver, uv3 };
	portVertices[6] = { pos5, green, gray, dark, silver, uv5 };
	portVertices[7] = { pos7, green, gray, dark, silver, uv7 };
	portVertices[8] = { pos8, green, gray, dark, silver, uv8 };

	portVertices[9] = { pos4, blue, darkGray, dark, silver, uv4 };
	portVertices[10] = { pos6, blue, darkGray, dark, silver, uv6 };
	portVertices[11] = { pos7, blue, darkGray, dark, silver, uv7 };
	portVertices[12] = { pos8, blue, darkGray, dark, silver, uv8 };

	vector<UINT> portIndices(21);

	portIndices[0] = 0;
	portIndices[1] = 1;
	portIndices[2] = 4;

	portIndices[3] = 0;
	portIndices[4] = 4;
	portIndices[5] = 2;

	portIndices[6] = 1;
	portIndices[7] = 3;
	portIndices[8] = 4;

	portIndices[9] = 5;
	portIndices[10] = 6;
	portIndices[11] = 8;

	portIndices[12] = 5;
	portIndices[13] = 8;
	portIndices[14] = 7;

	portIndices[15] = 9;
	portIndices[16] = 12;
	portIndices[17] = 10;

	portIndices[18] = 9;
	portIndices[19] = 11;
	portIndices[20] = 12;

	DirectXObjectPool::SetPolygonMesh("Port", make_shared<PolygonMesh>((void*) &portVertices[0], sizeof(PortVertex), portVertices.size(), &portIndices[0], portIndices.size()));
}


void ResourceInitializer::InitializeVertexShaders()
{
	D3D11_INPUT_ELEMENT_DESC backgroundGridLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("BackgroundGrid", make_shared<VertexShader>(L"Shaders\\BackgroundGrid.hlsl", backgroundGridLayout, ARRAYSIZE(backgroundGridLayout)));


	D3D11_INPUT_ELEMENT_DESC environmentLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Environment", make_shared<VertexShader>(L"Shaders\\Environment.hlsl", environmentLayout, ARRAYSIZE(environmentLayout)));


	D3D11_INPUT_ELEMENT_DESC radianceMapLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("RadianceMap", make_shared<VertexShader>(L"Shaders\\RadianceMap.hlsl", radianceMapLayout, ARRAYSIZE(radianceMapLayout)));


	D3D11_INPUT_ELEMENT_DESC brdfLUTLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("BRDFLUT", make_shared<VertexShader>(L"Shaders\\BRDFLUT.hlsl", brdfLUTLayout, ARRAYSIZE(brdfLUTLayout)));


	D3D11_INPUT_ELEMENT_DESC irradianceSmoothMapLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("IrradianceSmoothMap", make_shared<VertexShader>(L"Shaders\\IrradianceSmoothMap.hlsl", irradianceSmoothMapLayout, ARRAYSIZE(irradianceSmoothMapLayout)));


	D3D11_INPUT_ELEMENT_DESC irradianceRoughMapLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("IrradianceRoughMap", make_shared<VertexShader>(L"Shaders\\IrradianceRoughMap.hlsl", irradianceRoughMapLayout, ARRAYSIZE(irradianceRoughMapLayout)));


	D3D11_INPUT_ELEMENT_DESC irradianceMultiMapLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("IrradianceMultiMap", make_shared<VertexShader>(L"Shaders\\IrradianceMultiMap.hlsl", irradianceMultiMapLayout, ARRAYSIZE(irradianceMultiMapLayout)));


	D3D11_INPUT_ELEMENT_DESC pbsLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("PBS", make_shared<VertexShader>(L"Shaders\\PBS.hlsl", pbsLayout, ARRAYSIZE(pbsLayout)));


	D3D11_INPUT_ELEMENT_DESC textureQuadLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("TextureQuad", make_shared<VertexShader>(L"Shaders\\TextureQuadColor.hlsl", textureQuadLayout, ARRAYSIZE(textureQuadLayout)));


	D3D11_INPUT_ELEMENT_DESC textureFrameLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("TextureFrame", make_shared<VertexShader>(L"Shaders\\TextureFrameColor.hlsl", textureFrameLayout, ARRAYSIZE(textureFrameLayout)));


	D3D11_INPUT_ELEMENT_DESC portLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Port", make_shared<VertexShader>(L"Shaders\\Port.hlsl", portLayout, ARRAYSIZE(portLayout)));


	D3D11_INPUT_ELEMENT_DESC polylineLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Polyline", make_shared<VertexShader>(L"Shaders\\Polyline.hlsl", polylineLayout, ARRAYSIZE(polylineLayout)));
}


void ResourceInitializer::InitializePixelShaders()
{
	DirectXObjectPool::SetPixelShader("BackgroundGrid", make_shared<PixelShader>(L"Shaders\\BackgroundGrid.hlsl"));

	DirectXObjectPool::SetPixelShader("Environment", make_shared<PixelShader>(L"Shaders\\Environment.hlsl"));

	DirectXObjectPool::SetPixelShader("RadianceMap", make_shared<PixelShader>(L"Shaders\\RadianceMap.hlsl"));

	DirectXObjectPool::SetPixelShader("BRDFLUT", make_shared<PixelShader>(L"Shaders\\BRDFLUT.hlsl"));

	DirectXObjectPool::SetPixelShader("IrradianceSmoothMap", make_shared<PixelShader>(L"Shaders\\IrradianceSmoothMap.hlsl"));

	DirectXObjectPool::SetPixelShader("IrradianceRoughMap", make_shared<PixelShader>(L"Shaders\\IrradianceRoughMap.hlsl"));

	DirectXObjectPool::SetPixelShader("IrradianceMultiMap", make_shared<PixelShader>(L"Shaders\\IrradianceMultiMap.hlsl"));

	DirectXObjectPool::SetPixelShader("PBS", make_shared<PixelShader>(L"Shaders\\PBS.hlsl"));

	DirectXObjectPool::SetPixelShader("TextureQuadGrayscale", make_shared<PixelShader>(L"Shaders\\TextureQuadGrayscale.hlsl"));

	DirectXObjectPool::SetPixelShader("TextureQuadColor", make_shared<PixelShader>(L"Shaders\\TextureQuadColor.hlsl"));

	DirectXObjectPool::SetPixelShader("TextureFrameGrayscale", make_shared<PixelShader>(L"Shaders\\TextureFrameGrayscale.hlsl"));

	DirectXObjectPool::SetPixelShader("TextureFrameColor", make_shared<PixelShader>(L"Shaders\\TextureFrameColor.hlsl"));

	DirectXObjectPool::SetPixelShader("Port", make_shared<PixelShader>(L"Shaders\\Port.hlsl"));

	DirectXObjectPool::SetPixelShader("PolylineLines", make_shared<PixelShader>(L"Shaders\\Polyline.hlsl"));

	DirectXObjectPool::SetPixelShader("PolylinePoints", make_shared<PixelShader>(L"Shaders\\PolylinePoints.hlsl"));
}


void ResourceInitializer::InitializeRasterizerStates()
{
	DirectXObjectPool::SetRasterizerState("Basic", make_shared<RasterizerState>(D3D11_FILL_SOLID, D3D11_CULL_BACK));

	DirectXObjectPool::SetRasterizerState("CullFront", make_shared<RasterizerState>(D3D11_FILL_SOLID, D3D11_CULL_FRONT));
}


void ResourceInitializer::InitializeSamplerStates()
{
	DirectXObjectPool::SetSamplerState("LinearWrap", make_shared<SamplerState>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0));

	DirectXObjectPool::SetSamplerState("LinearClamp", make_shared<SamplerState>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0));
	
	DirectXObjectPool::SetSamplerState("AnisotropicWrap", make_shared<SamplerState>(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, 16));
}


void ResourceInitializer::InitializeConstantBuffers()
{
	DirectXObjectPool::SetConstantBuffer("Camera", make_shared<ConstantBuffer>(sizeof(CameraCB)));

	DirectXObjectPool::SetConstantBuffer("Environment", make_shared<ConstantBuffer>(sizeof(EnvironmentCB)));

	DirectXObjectPool::SetConstantBuffer("RadianceMap", make_shared<ConstantBuffer>(sizeof(RadianceMapCB)));

	DirectXObjectPool::SetConstantBuffer("IrradianceSmoothMap", make_shared<ConstantBuffer>(sizeof(IrradianceSmoothMapCB)));

	DirectXObjectPool::SetConstantBuffer("IrradianceRoughMap", make_shared<ConstantBuffer>(sizeof(IrradianceRoughMapCB)));

	DirectXObjectPool::SetConstantBuffer("IrradianceMultiMap", make_shared<ConstantBuffer>(sizeof(IrradianceMultiMapCB)));

	DirectXObjectPool::SetConstantBuffer("TextureFrame", make_shared<ConstantBuffer>(sizeof(TextureFrameCB)));

	DirectXObjectPool::SetConstantBuffer("Port", make_shared<ConstantBuffer>(sizeof(PortCB)));

	DirectXObjectPool::SetConstantBuffer("Polyline", make_shared<ConstantBuffer>(sizeof(PolyLineCB)));
}


void ResourceInitializer::InitializeRecources()
{
	InitializePolygonMeshes();
	InitializeVertexShaders();
	InitializePixelShaders();
	InitializeRasterizerStates();
	InitializeSamplerStates();
	InitializeConstantBuffers();
}