struct VertexIn
{
	float3 posL : POSITION;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float3 posW : POSITION;
};


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjInverse;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	float scale;
};


SamplerState linearWrapSampler : register(s2);


TextureCube cubeMap : register(t5);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProjCentered).xyww;
	vertexOut.posW = vertexIn.posL;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	float4 color = cubeMap.SampleLevel(linearWrapSampler, vertexOut.posW, 0);
	return color;
}