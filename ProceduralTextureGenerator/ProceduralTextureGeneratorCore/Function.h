#pragma once

#include "AtomicFunctions.h"
#include "NoiseFunctions.h"
#include "TextureMemory.h"


#define BASE_COLOR 0
#define METALLIC 1
#define ROUGHNESS 2
#define NORMAL 3
#define HEIGHT 4
#define UNIFORM_COLOR 5
#define BLEND 6
#define REMAP 7
#define GRADIENT 8
#define PERLIN_NOISE 9
#define WORLEY_NOISE 10
#define NORMAL_COLOR 11
#define BLUR 12
#define DIRECTIONAL_BLUR 13


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters);

int FunctionInputNodesNum(int functionIndex);

vector<int> FunctionIntParametersBase(int functionIndex);
vector<float> FunctionFloatParametersBase(int functionIndex);