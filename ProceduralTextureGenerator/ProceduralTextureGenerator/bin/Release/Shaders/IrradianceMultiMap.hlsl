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


cbuffer cbIrradianceMultiMap : register(b0)
{
	int faceIndex;
	int aligner1;
	int aligner2;
	int aligner3;
};


SamplerState anisotropicWrapSampler : register(s0);


TextureCube environmentMap : register(t0);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = float4(vertexIn.posL, 1.0f);
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}


float LinearToSRGB(float linearColor)
{
	return pow(linearColor, 0.454545);
}


float3 Direction(int face, float2 uv)
{
	float3 direction;
	float2 coords = 2.0f * uv - float2(1.0f, 1.0f);

	switch(face)
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


float3 ImportanceSample(float2 xi, float3 n)
{
	float phi = PI / 2.0f * xi.y;
	float psi = 2.0f * PI * xi.x;

	float cosPhi = cos(phi);
	float sinPhi = sin(phi);
	float cosPsi = cos(psi);
	float sinPsi = sin(psi);

	float3 h = float3(sinPhi * cosPsi, sinPhi * sinPsi, cosPhi);

	float3 up = abs(n.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);

	float3 t = normalize(cross(up, n));
	float3 b = cross(n, t);

	return t * h.x + b * h.y + n * h.z;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	int samplesNum = 2048;
	float3 r = Direction(faceIndex, vertexOut.uv);

	float weight = 0.0f;
	float3 color = float3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < samplesNum; i++)
	{
		float2 xi = Hammersley(i, samplesNum);
		float3 l = ImportanceSample(xi, r);
	
		float dotnl = saturate(dot(r, l));
	
		float _1dotnl1 = 1.0f - dotnl;
		float _1dotnl2 = _1dotnl1 * _1dotnl1;
		float _1dotnl5 = _1dotnl2 * _1dotnl2 * _1dotnl1;
	
		color += environmentMap.SampleLevel(anisotropicWrapSampler, l, 0).rgb * LinearToSRGB(0.1159f * dotnl);
		weight += dotnl;
	}
	
	return float4(color / samplesNum, 1.0f);
}