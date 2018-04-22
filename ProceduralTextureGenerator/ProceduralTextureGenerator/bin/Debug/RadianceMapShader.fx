static const float PI = 3.14159265f;


struct VertexIn
{
	float3 posL : POSITION;
	float2 uv : TEXCOORD;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 uv : TEXCOORD;
};


cbuffer cbRadianceMap : register(b0)
{
	float roughness;
	int faceIndex;
	int aligner1;
	int aligner2;
};


SamplerState anisotropicSampler : register(s0);


TextureCube environmentMap : register(t0);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = float4(vertexIn.posL, 1.0f);
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float3 SRGBToLinear(float3 sRGBColor)
{
	const float3 linearRGBLo = sRGBColor / 12.92f;
	const float3 linearRGBHi = pow((sRGBColor + float3(0.055f, 0.055f, 0.055f)) / 1.055f, 2.4f);
	const float3 linearRGB = (sRGBColor <= 0.04045) ? linearRGBLo : linearRGBHi;

	return linearRGB;
}


float3 LinearToSRGB(float3 linearColor)
{
	const float3 sRGBLo = linearColor * 12.92f;
	const float3 sRGBHi = pow(linearColor, 0.416667f) * 1.055f - float3(0.055f, 0.055f, 0.055f);
	const float3 sRGB = (linearColor <= 0.0031308) ? sRGBLo : sRGBHi;

	return sRGB;
}


float3 Direction(int face, float2 uv)
{
	float3 direction;
	float2 coords = 2.0f * uv - float2(1.0f, 1.0f);

	switch (face)
	{
		case 0:
		{
			direction = float3(1.0f, -coords.y, -coords.x);
			break;
		}
		case 1:
		{
			direction = float3(-1.0f, -coords.y, coords.x);
			break;
		}
		case 2:
		{
			direction = float3(coords.x, 1.0f, coords.y);
			break;
		}
		case 3:
		{
			direction = float3(coords.x, -1.0f, -coords.y);
			break;
		}
		case 4:
		{
			direction = float3(coords.x, -coords.y, 1.0f);
			break;
		}
		case 5:
		{
			direction = float3(-coords.x, -coords.y, -1.0f);
			break;
		}
		default:
		{
			direction = float3(0.0f, 0.0f, 0.0f);
			break;
		}
	};

	return normalize(direction);
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


float4 PS(VertexOut vertexOut) : SV_Target
{
	const int samplesNum = 1024;
	const float3 r = Direction(faceIndex, vertexOut.uv);

	float weight = 0.0f;
	float3 color = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < samplesNum; i++)
	{
		float2 xi = Hammersley(i, samplesNum);
		float3 h = ImportanceSampleGGX(xi, roughness, r);
		float3 l = 2 * dot(r, h) * h - r;

		float dotnl = saturate(dot(r, l));
		if (dotnl > 0)
		{
			color += environmentMap.SampleLevel(anisotropicSampler, l, 0).rgb * dotnl;
			weight += dotnl;
		}
	}

	return float4(color / weight, 1.0f);
}