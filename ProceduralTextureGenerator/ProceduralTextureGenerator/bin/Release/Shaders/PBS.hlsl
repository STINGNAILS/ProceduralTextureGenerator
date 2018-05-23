static const float PI = 3.14159265f;


struct DirectionalLight
{
	float4 intensity;
	float3 direction;
	int isEnabled;
};


struct SphereLight
{
	float3 intensity;
	float lightRadius;
	float3 position;
	float sourceRadius;
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


cbuffer cbCamera : register(b0)
{
	float4x4 viewProj;
	float4x4 viewProjInverse;
	float4x4 viewProjCentered;
	float3 cameraPosW;
	float scale;
};


cbuffer cbEnvironment : register(b1)
{
	int sphereLightsNum;
	int radianceMapMipLevelsFactor;
	int aligner4;
	int aligner5;
	DirectionalLight directionalLight;
	SphereLight sphereLights[4];
};


SamplerState anisotropicWrapSampler : register(s0);
SamplerState linearClampSampler : register(s1);
SamplerState linearWrapSampler : register(s2);


TextureCube radianceMap : register(t0);
Texture2D brdfLUT: register(t1);

TextureCube irradianceSmoothMap : register(t2);
TextureCube irradianceRoughMap : register(t3);
TextureCube irradianceMultiMap : register(t4);

Texture2D albedoMap : register(t5);
Texture2D metallicMap : register(t6);
Texture2D roughnessMap : register(t7);
Texture2D normalMap : register(t8);


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


//float3 LambertBRDF(float4 diffuse)
//{
//	return diffuse / PI;
//}
//
//
//float3 OrenNayarBRDF(float3 diffuse, float roughness, float dotnl, float dotnv, float dotvh)
//{
//	const float s2 = pow(roughness, 4);
//	const float dotlv = 2.0f * dotvh * dotvh - 1.0f;
//	const float cosphi = dotlv - dotnv * dotnl;
//	const float C1 = 1.0f - 0.5f * s2 / (s2 + 0.33f);
//	const float C2 = 0.45f * s2 / (s2 + 0.09f) * cosphi * (cosphi > 0 ? rcp(max(dotnl, dotnv)) : 1.0f);
//
//	return diffuse / PI * (C1 + C2);
//}
//
//
//float3 CookTorranceBRDF(float3 f0, float roughness, float dotnl, float dotnv, float dotnh, float dotvh)
//{
//	if (dotnl > 0.0f)
//	{
//		const float alpha2 = pow(roughness, 4);
//		const float D = alpha2 / (PI * pow((dotnh * dotnh * (alpha2 - 1) + 1), 2));
//
//		const float3 F = f0 + (float3(1.0f, 1.0f, 1.0f) - f0) * exp2((-5.55473 * dotvh - 6.98316) * dotvh);
//
//		const float k = pow(roughness + 1, 2) / 8;
//		const float G1l = 1 / (dotnl * (1 - k) + k);
//		const float G1v = 1 / (dotnv * (1 - k) + k);
//		const float G = G1l * G1v;
//
//		return D * F * G / 4;
//	}
//
//	return float3(0.0f, 0.0f, 0.0f);
//}


float3 SpecularBRDF(float3 f0, float alpha, float dotnl, float dotnv, float dotnh, float dotvh)
{
	float _1dotvh1 = 1.0f - dotvh;
	float _1dotvh2 = _1dotvh1 * _1dotvh1;
	float _1dotvh5 = _1dotvh2 * _1dotvh2 * _1dotvh1;

	float alpha2 = alpha * alpha;
	float d1 = (1.0f - (1.0f - alpha2) * dotnh * dotnh);
	float D = alpha2 / (PI * d1 * d1);

	float3 F = f0 + (float3(1.0f, 1.0f, 1.0f) - f0) * _1dotvh5;

	float G = 0.5f / lerp(2.0f * dotnl * dotnv, dotnl + dotnv, alpha);

	return D * G * F;
}


float3 DiffuseBRDF(float3 albedo, float alpha, float dotnl, float dotnv, float dotnh, float dotlv)
{
	float _1dotnl1 = 1.0f - dotnl;
	float _1dotnl2 = _1dotnl1 * _1dotnl1;
	float _1dotnl5 = _1dotnl2 * _1dotnl2 * _1dotnl1;

	float _1dotnv1 = 1.0f - dotnv;
	float _1dotnv2 = _1dotnv1 * _1dotnv1;
	float _1dotnv5 = _1dotnv2 * _1dotnv2 * _1dotnv1;

	float facing = 0.5f + 0.5f * dotlv;
	float smooth = 1.05f * (1.0f - _1dotnl5) * (1.0f - _1dotnv5);
	float rough = facing * (0.9f - 0.4f * facing) * (0.5f + dotnh) / dotnh;
	float single = lerp(smooth, rough, alpha) / PI;
	float multi = 0.1159f * alpha;

	return albedo * (single + albedo * multi);
}


float3 SpecularIBL(float3 f0, float roughness, float3 n, float3 v)
{
	float dotnv = saturate(dot(n, v));
	float3 r = 2 * dot(n, v) * n - v;

	float3 prefilteredColor = SRGBToLinear(radianceMap.SampleLevel(anisotropicWrapSampler, r, roughness * radianceMapMipLevelsFactor));
	float2 environmentBRDF = brdfLUT.Sample(linearClampSampler, float2(roughness, dotnv));

	return prefilteredColor * (f0 * environmentBRDF.x + float3(environmentBRDF.y, environmentBRDF.y, environmentBRDF.y));
}


float3 DiffuseIBL(float3 albedo, float roughness, float3 n, float3 v)
{
	float alpha = roughness * roughness;
	float dotnv = saturate(dot(n, v));

	float _1dotnv1 = 1.0f - dotnv;
	float _1dotnv2 = _1dotnv1 * _1dotnv1;
	float _1dotnv5 = _1dotnv2 * _1dotnv2 * _1dotnv1;

	float3 smooth = SRGBToLinear(irradianceSmoothMap.SampleLevel(anisotropicWrapSampler, n, 0));
	float3 rough = SRGBToLinear(irradianceRoughMap.SampleLevel(anisotropicWrapSampler, n, 0));
	float3 multi = SRGBToLinear(irradianceMultiMap.SampleLevel(anisotropicWrapSampler, n, 0));

	return albedo * (lerp(smooth * (1 - _1dotnv5), rough, alpha) + albedo * alpha * multi);
}


float Falloff(float distance, float radius)
{
	float dr1 = distance / radius;
	float dr2 = dr1 * dr1;
	float dr4 = dr2 * dr2;

	float _1dr4 = saturate(1.0f - dr4);

	return _1dr4 * _1dr4 / (distance * distance + 1.0f);
}


float Normalization(float alpha2, float sourceRadius, float distance, float dotnv)
{
	double alpha2_ = alpha2 + saturate((sourceRadius * sourceRadius) / (distance * distance)) / (dotnv * 3.6f + 0.4f);
	return max(alpha2 / alpha2_, 0.01f);
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	float3 baseColor = SRGBToLinear(albedoMap.Sample(linearWrapSampler, vertexOut.uv).xyz);
	float metallic = saturate(1.190476f * metallicMap.Sample(linearWrapSampler, vertexOut.uv) - 0.095238f);
	float roughness = max(roughnessMap.Sample(linearWrapSampler, vertexOut.uv), 0.01f);
	float3 normal = 2.0f * normalMap.Sample(linearWrapSampler, vertexOut.uv).rgb - 1.0f;

	float3 albedo = lerp(baseColor, float3(0.0f, 0.0f, 0.0f), metallic);
	float3 f0 = lerp(float3(0.04f, 0.04f, 0.04f), baseColor, metallic);

	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;

	float3 N = normalize(vertexOut.normalW);
	float3 T = normalize(vertexOut.tangentW - dot(vertexOut.tangentW, N) * N);
	float3 B = normalize(cross(N, T));

	float3x3 TBN = float3x3(T, B, N);

	float3 n = normalize(mul(normal, TBN));
	float3 v = normalize(cameraPosW - vertexOut.posW);

	float dotnv = max(dot(n, v), 0.0f);

	float3 color = float3(0.0f, 0.0f, 0.0f);

	if(directionalLight.isEnabled)
	{
		float3 l = normalize(-directionalLight.direction);
		float3 h = normalize(l + v);

		float dotnl = max(dot(n, l), 0.0f);
		float dotnh = max(dot(n, h), 0.0f);
		float dotvh = max(dot(v, h), 0.0f);
		float dotlv = dot(l, v);

		if(dotnl * dotnv > 0.0f)
		{
			float3 specularBRDF = SpecularBRDF(f0, alpha, dotnl, dotnv, dotnh, dotvh);
			float3 diffuseBRDF = DiffuseBRDF(albedo, alpha, dotnl, dotnv, dotnh, dotlv);

			color += directionalLight.intensity.rgb * (specularBRDF + diffuseBRDF) * dotnl;
		}
	}

	for(int i = 0; i < sphereLightsNum; i++)
	{
		float3 L = sphereLights[i].position - vertexOut.posW;

		float3 r = 2.0f * dotnv * n - v;
		float3 specularDominantDirection = normalize(lerp(n, r, (1 - alpha) * (sqrt(1 - alpha) + alpha)));
		float3 centerToRaySpecular = dot(L, specularDominantDirection) * specularDominantDirection - L;
		float centerToRayLength = length(centerToRaySpecular);
		float3 representationPointSpecular = L + centerToRaySpecular * saturate(sphereLights[i].sourceRadius / centerToRayLength);

		float3 lS = normalize(representationPointSpecular);
		float3 hS = normalize(lS + v);

		float dotnlS = max(dot(n, lS), 0.0f);
		float dotnhS = alpha < 0.0025f && centerToRayLength < sphereLights[i].sourceRadius ? 1.0f : max(dot(n, hS), 0.0f);
		float dotvhS = max(dot(v, hS), 0.0f);

		if(dotnlS * dotnv > 0.0f)
		{
			float distance = length(representationPointSpecular);
			float falloff = Falloff(distance, sphereLights[i].lightRadius);
			float normalization = Normalization(alpha2, sphereLights[i].sourceRadius, distance, dotnv);
			float3 specularBRDF = SpecularBRDF(f0, alpha, dotnlS, dotnv, dotnhS, dotvhS);

			color += sphereLights[i].intensity * falloff * normalization * specularBRDF * dotnlS;
		}


		float3 diffuseDominantDirection = normalize(lerp(n, normalize(n + v), alpha));
		float3 centerToRayDiffuse = dot(L, diffuseDominantDirection) * diffuseDominantDirection - L;
		float3 representationPointDiffuse = L + centerToRayDiffuse * saturate(sphereLights[i].sourceRadius / length(centerToRayDiffuse));

		float3 lD = normalize(representationPointDiffuse);
		float3 hD = normalize(lD + v);

		float dotnlD = max(dot(n, lD), 0.0f);
		float dotnhD = max(dot(n, hD), 0.0f);
		float dotlvD = dot(lD, v);

		if(dotnlD * dotnv > 0.0f)
		{
			float distance = length(representationPointDiffuse);
			float falloff = Falloff(distance, sphereLights[i].lightRadius);
			float normalization = Normalization(alpha2, sphereLights[i].sourceRadius, distance, dotnv);
			float3 diffuseBRDF = DiffuseBRDF(albedo, alpha, dotnlD, dotnv, dotnhD, dotlvD);

			color += sphereLights[i].intensity * falloff * normalization * diffuseBRDF * dotnlD;
		}
	}

	float3 colorSpecIndirect = SpecularIBL(f0, roughness, n, v);
	float3 colorDiffIndirect = DiffuseIBL(albedo, roughness, n, v);

	color += colorSpecIndirect + colorDiffIndirect;

	return float4(LinearToSRGB(color), 1.0f);
}