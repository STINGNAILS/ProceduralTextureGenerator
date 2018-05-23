struct VertexIn
{
	float3 posL : POSITION;
	float4 colorColor : COLOR0;
	float4 colorGrayscale : COLOR1;
	float4 colorMandatoryNotLinked : COLOR2;
	float4 colorNotMandatoryOrLinked : COLOR3;
	float2 uv : TEXCOORD;
};


struct VertexOut
{
	float4 posH :SV_POSITION;
	float4 colorOuter : COLOR0;
	float4 colorInner : COLOR1;
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
	float ratio;
	float isGrayscale;
	float isMandatoryNotLinked;
	float aligner4;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(mul(float4(vertexIn.posL, 1.0f), world), viewProj);
	vertexOut.colorOuter = lerp(vertexIn.colorColor, vertexIn.colorGrayscale, isGrayscale);
	vertexOut.colorInner = lerp(vertexIn.colorNotMandatoryOrLinked, vertexIn.colorMandatoryNotLinked, isMandatoryNotLinked);
	vertexOut.uv = vertexIn.uv;
	
	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	float r = sqrt(vertexOut.uv.x * vertexOut.uv.x + vertexOut.uv.y * vertexOut.uv.y);
	clip(1.0f - r);

	return lerp(vertexOut.colorInner, vertexOut.colorOuter, step(ratio, r));
}