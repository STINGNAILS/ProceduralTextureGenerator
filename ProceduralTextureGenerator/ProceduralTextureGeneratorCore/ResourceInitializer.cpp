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


struct PinVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color0;
	XMFLOAT4 color1;
	XMFLOAT4 color2;
	XMFLOAT4 color3;
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


	vector<PinVertex> pinVertices(13);

	XMFLOAT3 pos1 = XMFLOAT3(-OutputPin::r0, OutputPin::r0, 0.0f);
	XMFLOAT3 pos2 = XMFLOAT3(OutputPin::r0, OutputPin::r0, 0.0f);
	XMFLOAT3 pos3 = XMFLOAT3(-OutputPin::r0, -OutputPin::r0, 0.0f);
	XMFLOAT3 pos4 = XMFLOAT3(OutputPin::r0, -OutputPin::r0, 0.0f);
	XMFLOAT3 pos5 = XMFLOAT3(-OutputPin::r0, 0.577350f * OutputPin::r0, 0.0f);
	XMFLOAT3 pos6 = XMFLOAT3(OutputPin::r0, 0.577350f * OutputPin::r0, 0.0f);
	XMFLOAT3 pos7 = XMFLOAT3(0.0f, -OutputPin::r0, 0.0f);
	XMFLOAT3 pos8 = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT4 red0 = XMFLOAT4(0.75f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green0 = XMFLOAT4(0.0f, 0.75f, 0.0f, 1.0f);
	XMFLOAT4 blue0 = XMFLOAT4(0.0f, 0.0f, 0.75f, 1.0f);
	XMFLOAT4 gray = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
	XMFLOAT4 red1 = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green1 = XMFLOAT4(0.0f, 0.1f, 0.0f, 1.0f);
	XMFLOAT4 blue1 = XMFLOAT4(0.0f, 0.0f, 0.1f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	XMFLOAT2 uv1 = XMFLOAT2(-1.0f, 1.0f);
	XMFLOAT2 uv2 = XMFLOAT2(1.0f, 1.0f);
	XMFLOAT2 uv3 = XMFLOAT2(-1.0f, -1.0f);
	XMFLOAT2 uv4 = XMFLOAT2(1.0f, -1.0f);
	XMFLOAT2 uv5 = XMFLOAT2(-1.0f, 0.577350f);
	XMFLOAT2 uv6 = XMFLOAT2(1.0f, 0.577350f);
	XMFLOAT2 uv7 = XMFLOAT2(0.0f, -1.0f);
	XMFLOAT2 uv8 = XMFLOAT2(0.0f, 0.0f);

	pinVertices[0] = { pos1, red0, red1, gray, white, uv1 };
	pinVertices[1] = { pos2, red0, red1, gray, white, uv2 };
	pinVertices[2] = { pos5, red0, red1, gray, white, uv5 };
	pinVertices[3] = { pos6, red0, red1, gray, white, uv6 };
	pinVertices[4] = { pos8, red0, red1, gray, white, uv8 };

	pinVertices[5] = { pos3, green0, green1, gray, white, uv3 };
	pinVertices[6] = { pos5, green0, green1, gray, white, uv5 };
	pinVertices[7] = { pos7, green0, green1, gray, white, uv7 };
	pinVertices[8] = { pos8, green0, green1, gray, white, uv8 };

	pinVertices[9] = { pos4, blue0, blue1, gray, white, uv4 };
	pinVertices[10] = { pos6, blue0, blue1, gray, white, uv6 };
	pinVertices[11] = { pos7, blue0, blue1, gray, white, uv7 };
	pinVertices[12] = { pos8, blue0, blue1, gray, white, uv8 };

	vector<UINT> pinIndices(21);

	pinIndices[0] = 0;
	pinIndices[1] = 1;
	pinIndices[2] = 4;

	pinIndices[3] = 0;
	pinIndices[4] = 4;
	pinIndices[5] = 2;

	pinIndices[6] = 1;
	pinIndices[7] = 3;
	pinIndices[8] = 4;

	pinIndices[9] = 5;
	pinIndices[10] = 6;
	pinIndices[11] = 8;

	pinIndices[12] = 5;
	pinIndices[13] = 8;
	pinIndices[14] = 7;

	pinIndices[15] = 9;
	pinIndices[16] = 12;
	pinIndices[17] = 10;

	pinIndices[18] = 9;
	pinIndices[19] = 11;
	pinIndices[20] = 12;

	DirectXObjectPool::SetPolygonMesh("Pin", make_shared<PolygonMesh>((void*) &pinVertices[0], sizeof(PinVertex), pinVertices.size(), &pinIndices[0], pinIndices.size()));
}


void ResourceInitializer::InitializeVertexShaders()
{
	D3D11_INPUT_ELEMENT_DESC backgroundGridLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("BackgroundGrid", make_shared<VertexShader>(L"BackgroundGrid.fx", backgroundGridLayout, ARRAYSIZE(backgroundGridLayout)));


	D3D11_INPUT_ELEMENT_DESC environmentLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Environment", make_shared<VertexShader>(L"EnvironmentShader.fx", environmentLayout, ARRAYSIZE(environmentLayout)));


	D3D11_INPUT_ELEMENT_DESC radianceMapLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("RadianceMap", make_shared<VertexShader>(L"RadianceMapShader.fx", radianceMapLayout, ARRAYSIZE(radianceMapLayout)));


	D3D11_INPUT_ELEMENT_DESC brdfLUTLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("BRDFLUT", make_shared<VertexShader>(L"BRDFLUTShader.fx", brdfLUTLayout, ARRAYSIZE(brdfLUTLayout)));


	D3D11_INPUT_ELEMENT_DESC pbsLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("PBS", make_shared<VertexShader>(L"PBS.fx", pbsLayout, ARRAYSIZE(pbsLayout)));


	D3D11_INPUT_ELEMENT_DESC textureQuadLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("TextureQuad", make_shared<VertexShader>(L"TextureQuadColor.fx", textureQuadLayout, ARRAYSIZE(textureQuadLayout)));


	D3D11_INPUT_ELEMENT_DESC textureFrameLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectXObjectPool::SetVertexShader("TextureFrame", make_shared<VertexShader>(L"TextureFrameColor.fx", textureFrameLayout, ARRAYSIZE(textureFrameLayout)));


	D3D11_INPUT_ELEMENT_DESC pinLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Pin", make_shared<VertexShader>(L"OutputPin.fx", pinLayout, ARRAYSIZE(pinLayout)));


	D3D11_INPUT_ELEMENT_DESC polylineLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	DirectXObjectPool::SetVertexShader("Polyline", make_shared<VertexShader>(L"Polyline.fx", polylineLayout, ARRAYSIZE(polylineLayout)));
}


void ResourceInitializer::InitializePixelShaders()
{
	DirectXObjectPool::SetPixelShader("BackgroundGrid", make_shared<PixelShader>(L"BackgroundGrid.fx"));

	DirectXObjectPool::SetPixelShader("Environment", make_shared<PixelShader>(L"EnvironmentShader.fx"));

	DirectXObjectPool::SetPixelShader("RadianceMap", make_shared<PixelShader>(L"RadianceMapShader.fx"));

	DirectXObjectPool::SetPixelShader("BRDFLUT", make_shared<PixelShader>(L"BRDFLUTShader.fx"));

	DirectXObjectPool::SetPixelShader("PBS", make_shared<PixelShader>(L"PBS.fx"));

	DirectXObjectPool::SetPixelShader("TextureQuadGrayscale", make_shared<PixelShader>(L"TextureQuadGrayscale.fx"));

	DirectXObjectPool::SetPixelShader("TextureQuadColor", make_shared<PixelShader>(L"TextureQuadColor.fx"));

	DirectXObjectPool::SetPixelShader("TextureFrameGrayscale", make_shared<PixelShader>(L"TextureFrameGrayscale.fx"));

	DirectXObjectPool::SetPixelShader("TextureFrameColor", make_shared<PixelShader>(L"TextureFrameColor.fx"));

	DirectXObjectPool::SetPixelShader("Pin", make_shared<PixelShader>(L"OutputPin.fx"));

	DirectXObjectPool::SetPixelShader("PolylineLines", make_shared<PixelShader>(L"Polyline.fx"));

	DirectXObjectPool::SetPixelShader("PolylinePoints", make_shared<PixelShader>(L"PolylinePoints.fx"));
}


void ResourceInitializer::InitializeRasterizerStates()
{
	DirectXObjectPool::SetRasterizerState("Basic", make_shared<RasterizerState>(D3D11_FILL_SOLID, D3D11_CULL_BACK));

	DirectXObjectPool::SetRasterizerState("CullFront", make_shared<RasterizerState>(D3D11_FILL_SOLID, D3D11_CULL_FRONT));
}


void ResourceInitializer::InitializeSamplerStates()
{
	DirectXObjectPool::SetSamplerState("Basic", make_shared<SamplerState>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0));

	DirectXObjectPool::SetSamplerState("Anisotropic", make_shared<SamplerState>(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, 16));
}


void ResourceInitializer::InitializeConstantBuffers()
{
	DirectXObjectPool::SetConstantBuffer("Camera", make_shared<ConstantBuffer>(sizeof(CameraCB)));

	DirectXObjectPool::SetConstantBuffer("Environment", make_shared<ConstantBuffer>(sizeof(EnvironmentCB)));

	DirectXObjectPool::SetConstantBuffer("RadianceMap", make_shared<ConstantBuffer>(sizeof(RadianceMapCB)));

	DirectXObjectPool::SetConstantBuffer("TextureFrame", make_shared<ConstantBuffer>(sizeof(TextureFrameCB)));

	DirectXObjectPool::SetConstantBuffer("Pin", make_shared<ConstantBuffer>(sizeof(OutputPinCB)));

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