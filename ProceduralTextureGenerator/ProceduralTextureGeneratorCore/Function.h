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
#define LEVELS 7
#define GRADIENT 8
#define PERLIN_NOISE 9
#define NORMAL_COLOR 10


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters);

int FunctionInputNodesNum(int functionIndex);

vector<int> FunctionIntParametersBase(int functionIndex);
vector<float> FunctionFloatParametersBase(int functionIndex);