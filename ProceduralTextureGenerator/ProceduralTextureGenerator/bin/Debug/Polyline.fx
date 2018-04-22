struct VertexIn
{
	float3 posL : POSITION;
	float4 color0: COLOR0;
	float4 color1: COLOR1;
	float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color: COLOR;
	float2 uv : TEXCOORD;
};


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjInverse;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	float scale;
};


cbuffer cbPolyline : register(b1)
{
	float state;
	int aligner4;
	int aligner5;
	int aligner6;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.color = (1.0f - state) * vertexIn.color0 + state * vertexIn.color1;
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	return vertexOut.color;
}