struct VertexIn
{
	float3 posL : POSITION;
	float4 col0 : COLOR0;
	float4 col1 : COLOR1;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float4 col : COLOR;
};


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	int aligner1;
};


cbuffer cbInputPin : register(b1)
{
	float4x4 world;
	float state;
	float aligner2;
	float aligner3;
	float aligner4;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(mul(float4(vertexIn.posL, 1.0f), world), viewProj);
	vertexOut.col = (1.0f - state) * vertexIn.col0 + state * vertexIn.col1;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	return vertexOut.col;
}