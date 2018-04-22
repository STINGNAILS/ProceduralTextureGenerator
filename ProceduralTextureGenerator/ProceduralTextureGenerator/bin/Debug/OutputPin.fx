struct VertexIn
{
	float3 posL : POSITION;
	float4 col0 : COLOR0;
	float4 col1 : COLOR1;
	float4 col2 : COLOR2;
	float4 col3 : COLOR3;
	float2 uv : TEXCOORD;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float4 col0 : COLOR0;
	float4 col1 : COLOR1;
	float4 col2 : COLOR2;
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


cbuffer cbInputPin : register(b1)
{
	float4x4 world;
	float state;
	float ratio;
	float aligner4;
	float aligner5;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(mul(float4(vertexIn.posL, 1.0f), world), viewProj);
	vertexOut.col0 = vertexIn.col0;
	vertexOut.col1 = vertexIn.col1;
	vertexOut.col2 = state * vertexIn.col3 + (1.0f - state) * vertexIn.col2;
	vertexOut.uv = vertexIn.uv;
	
	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	float r = sqrt(vertexOut.uv.x * vertexOut.uv.x + vertexOut.uv.y * vertexOut.uv.y);
	clip(1.0f - r);
	float k0 = 1.0f - step(1.0001f, r);
	float k1 = 1.0f - step(ratio, r);
	float4 color1 = (1.0f - state) * (r * vertexOut.col0 + (1.0f - r) * vertexOut.col1) + state * vertexOut.col0;
	float4 color2 = vertexOut.col2;

	return (k0 - k1) * color1 + k1 * color2;
}