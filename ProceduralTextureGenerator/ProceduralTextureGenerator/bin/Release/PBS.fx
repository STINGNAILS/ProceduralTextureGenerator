static const float PI = 3.14159265f;


struct DirectionalLight
{
	float4 intensity;
	float4 direction;
};


struct VertexIn
{
	float3 posL : POSITION;
	float3 normalL : NORMAL;
	float3 tangentL : TANGENT;
	float2 uv : TEXCOORD;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float2 uv : TEXCOORD;
};


SamplerState basicSampler
{
	Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
};


SamplerState anisotropicSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 16;
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


cbuffer cbEnvironment : register(b1)
{
	int directionalLightsNum;
	int radianceMapMipLevelsFactor;
	int aligner2;
	int aligner3;
	DirectionalLight directionalLights[4];
};


TextureCube radianceMap : register(t0);
Texture2D brdfLUT: register(t1);

Texture2D albedoMap : register(t2);
Texture2D metallicMap : register(t3);
Texture2D roughnessMap : register(t4);
Texture2D normalMap : register(t5);


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;
	
	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.posW = vertexIn.posL;
	vertexOut.normalW = vertexIn.normalL;
	vertexOut.tangentW = vertexIn.tangentL;
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


float3 LambertBRDF(float4 diffuse)
{
	return diffuse / PI;
}


float3 OrenNayarBRDF(float3 diffuse, float roughness, float dotnl, float dotnv, float dotvh)
{
	const float s2 = pow(roughness, 4);
	const float dotlv = 2.0f * dotvh * dotvh - 1.0f;
	const float cosphi = dotlv - dotnv * dotnl;
	const float C1 = 1.0f - 0.5f * s2 / (s2 + 0.33f);
	const float C2 = 0.45f * s2 / (s2 + 0.09f) * cosphi * (cosphi > 0 ? rcp(max(dotnl, dotnv)) : 1.0f);

	return diffuse / PI * (C1 + C2);
}


float3 CookTorranceBRDF(float3 f0, float roughness, float dotnl, float dotnv, float dotnh, float dotvh)
{
	if (dotnl > 0.0f)
	{
		const float alpha2 = pow(roughness, 4);
		const float D = alpha2 / (PI * pow((dotnh * dotnh * (alpha2 - 1) + 1), 2));

		const float3 F = f0 + (float3(1.0f, 1.0f, 1.0f) - f0) * exp2((-5.55473 * dotvh - 6.98316) * dotvh);

		const float k = pow(roughness + 1, 2) / 8;
		const float G1l = 1 / (dotnl * (1 - k) + k);
		const float G1v = 1 / (dotnv * (1 - k) + k);
		const float G = G1l * G1v;

		return D * F * G / 4;
	}

	return float3(0.0f, 0.0f, 0.0f);
}


float3 SpecularIBL(float3 specularColor, float roughness, float3 n, float3 v)
{
	float dotnv = saturate(dot(n, v));
	float3 r = 2 * dot(n, v) * n - v;

	float3 prefilteredColor = SRGBToLinear(radianceMap.SampleLevel(anisotropicSampler, r, roughness * radianceMapMipLevelsFactor));
	float2 environmentBRDF = brdfLUT.Sample(basicSampler, float2(roughness, dotnv));

	return prefilteredColor * (specularColor * environmentBRDF.x + environmentBRDF.y);
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	const float3 albedo = SRGBToLinear(albedoMap.Sample(basicSampler, vertexOut.uv).xyz);
	const float metallic = saturate(1.190476f * metallicMap.Sample(basicSampler, vertexOut.uv) - 0.095238f);
	const float roughness = max(roughnessMap.Sample(basicSampler, vertexOut.uv), 0.01f);
	const float3 normal = 2.0f * normalMap.Sample(basicSampler, vertexOut.uv).rgb - 1.0f;

	const float3 diffuse = lerp(albedo, float3(0.0f, 0.0f, 0.0f), metallic);
	const float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);

	const float3 N = normalize(vertexOut.normalW);
	const float3 T = normalize(vertexOut.tangentW - dot(vertexOut.tangentW, N) * N);
	const float3 B = normalize(cross(N, T));

	float3x3 TBN = float3x3(T, B, N);

	const float3 n = normalize(mul(normal, TBN));
	const float3 v = normalize(cameraPosW - vertexOut.posW);

	const float dotnv = max(dot(n, v), 0.0f);

	float3 color = float3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < directionalLightsNum; i++)
	{
		const float3 l = normalize(-directionalLights[i].direction);
		const float3 h = normalize(l + v);

		const float dotnl = max(dot(n, l), 0.0f);
		const float dotnh = max(dot(n, h), 0.0f);
		const float dotvh = max(dot(v, h), 0.0f);

		//float4 colorDiff = LambertBRDF(diffuse);
		float3 colorDiff = OrenNayarBRDF(diffuse, roughness, dotnl, dotnv, dotvh) * PI;
		float3 colorSpec = CookTorranceBRDF(f0, roughness, dotnl, dotnv, dotnh, dotvh);

		color += directionalLights[i].intensity * (colorDiff + colorSpec) * dotnl;
	}

	float3 colorSpecIndirect = SpecularIBL(f0, roughness, n, v);

	color += colorSpecIndirect;

	return float4(LinearToSRGB(color), 1.0f);
}