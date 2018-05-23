static const float PI = 3.14159265f;


struct VertexIn
{
	float3 posL : POSITION;
	float2 uv: TEXCOORD;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 uv: TEXCOORD;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = float4(vertexIn.posL, 1.0f);
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float2 Hammersley(int i, int num)
{
	float ri = reversebits(i) * 2.3283064365386963e-10f;
	return float2(float(i) / float(num), ri);
}


float3 ImportanceSampleGGX(float2 xi, float roughness, float3 n)
{
	float a = roughness * roughness;
	float phi = 2.0f * PI * xi.x;

	float cosTheta = sqrt((1.0f - xi.y) / (1.0f + (a * a - 1.0f) * xi.y));
	float sinTheta = sqrt(1 - cosTheta * cosTheta);

	float3 h = float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	float3 up = abs(n.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);

	float3 t = normalize(cross(up, n));
	float3 b = cross(n, t);

	return t * h.x + b * h.y + n * h.z;
}


float2 PS(VertexOut vertexOut) : SV_Target
{
	const float roughness = vertexOut.uv.x;
	const float dotnv = vertexOut.uv.y;

	const float3 n = float3(0.0f, 0.0f, 1.0f);
	const float3 v = float3(sqrt(1.0f - dotnv * dotnv), 0.0f, dotnv);

	float a = 0.0f;
	float b = 0.0f;

	const int samplesNum = 1024;

	for (int i = 0; i < samplesNum; i++)
	{
		float2 xi = Hammersley(i, samplesNum);
		float3 h = ImportanceSampleGGX(xi, roughness, n);
		float3 l = 2.0f * dot(v, h) * h - v;

		float dotnl = saturate(l.z);
		float dotnh = saturate(h.z);
		float dotvh = saturate(dot(v, h));

		if (dotnl > 0)
		{
			const float k = roughness * roughness / 2.0f;
			const float G1l = dotnl / (dotnl * (1.0f - k) + k);
			const float G1v = dotnv / (dotnv * (1.0f - k) + k);
			const float G = G1l * G1v * dotvh / (dotnh * dotnv);

			const float fc = pow(1.0f - dotvh, 5);

			a += (1.0f - fc) * G;
			b += fc * G;
		}
	}
	return float2(a / samplesNum, b / samplesNum);
}