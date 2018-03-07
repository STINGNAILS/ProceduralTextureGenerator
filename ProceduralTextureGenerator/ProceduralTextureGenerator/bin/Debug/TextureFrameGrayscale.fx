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


cbuffer cbFunctionNode : register(b1)
{
	float4x4 world;
	float isSelected;
	int aligner2;
	int aligner3;
	int aligner4;
};


Texture2D functionTexture : register(t0);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(mul(float4(vertexIn.posL, 1.0f), world), viewProj);
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	float k = step(0.05f, vertexOut.uv.x) * step(0.05f, 1.0f - vertexOut.uv.x) * step(0.05f, vertexOut.uv.y) * step(0.05f, 1.0f - vertexOut.uv.y);
	float4 frameColor = (1.0f - isSelected) * float4(0.1f, 0.1f, 0.1f, 1.0f) + isSelected * float4(0.9f, 0.9f, 0.9f, 1.0f);
	float4 color = (1.0f - k) * frameColor + k * functionTexture.SampleLevel(basicSampler, vertexOut.uv, 0);

	return float4(color.rrr, 1.0f);
}