struct VertexIn
{
	float3 posL : POSITION;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float3 posW : TEXCOORD;
};


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjInverse;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	float scale;
};


SamplerState linearWrapSampler : register(s0);


Texture2D backgroundGridTexture : register(t0);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = float4(vertexIn.posL, 1.0f);
	vertexOut.posW = mul(float4(vertexIn.posL, 1.0f), viewProjInverse);

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	return float4(backgroundGridTexture.SampleLevel(linearWrapSampler, (vertexOut.posW.xy - float2(32.0f, 32.0f)) / 64.0f, 1.0f + log2(1.0f / scale)).rrr, 1.0f);
}