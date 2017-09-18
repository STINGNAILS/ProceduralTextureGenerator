#pragma once

#include "DirectXDependencies.h"


using namespace DirectX;


XMFLOAT3 CalculateNormal(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3);
XMFLOAT3 CalculateTangent(XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT2 uv1, XMFLOAT2 uv2, XMFLOAT2 uv3);