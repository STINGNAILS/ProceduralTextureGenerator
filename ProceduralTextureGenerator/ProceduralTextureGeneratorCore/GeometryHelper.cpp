#include "stdafx.h"
#include "GeometryHelper.h"


XMFLOAT3 CalculateNormal(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3)
{
	XMFLOAT3 e1 = XMFLOAT3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	XMFLOAT3 e2 = XMFLOAT3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	XMVECTOR normalV = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&e2), XMLoadFloat3(&e1)));

	XMFLOAT3 normal;
	XMStoreFloat3(&normal, normalV);

	return normal;
}


XMFLOAT3 CalculateTangent(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT2 uv1, XMFLOAT2 uv2, XMFLOAT2 uv3)
{
	XMFLOAT3 e1 = XMFLOAT3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	XMFLOAT3 e2 = XMFLOAT3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	XMFLOAT2 duv1 = XMFLOAT2(uv2.x - uv1.x, uv2.y - uv1.y);
	XMFLOAT2 duv2 = XMFLOAT2(uv3.x - uv1.x, uv3.y - uv1.y);

	XMFLOAT3 tangent;

	float det = duv1.x * duv2.y - duv1.y * duv2.x;

	tangent.x = (duv2.y * e1.x - duv1.y * e2.x) / det;
	tangent.y = (duv2.y * e1.y - duv1.y * e2.y) / det;
	tangent.z = (duv2.y * e1.z - duv1.y * e2.z) / det;

	return tangent;
}