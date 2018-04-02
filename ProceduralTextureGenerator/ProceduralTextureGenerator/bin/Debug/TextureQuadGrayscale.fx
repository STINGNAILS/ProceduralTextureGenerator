struct VertexIn
{
	float3 posL : POSITION;
	float2 uv : TEXCOORD;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float2 uv : TEXCOORD;
};


SamplerState basicSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	int aligner1;
};


Texture2D functionTexture : register(t0);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	return float4(functionTexture.SampleLevel(basicSampler, vertexOut.uv, 0).rrr, 1.0f);
}