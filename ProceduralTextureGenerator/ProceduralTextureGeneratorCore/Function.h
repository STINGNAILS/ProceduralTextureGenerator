#pragma once

#include "AtomicFunctions.h"
#include "NoiseFunctions.h"
#include "TextureMemory.h"


#define BASE_COLOR 0
#define BASE_GRAYSCALE 1
#define UNIFORM_COLOR 2
#define BLEND 3
#define LEVELS 4
#define GRADIENT 5
#define PERLIN_NOISE 6
#define NORMAL_COLOR 7


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters);

int FunctionInputNodesNum(int functionIndex);

vector<int> FunctionIntParametersBase(int functionIndex);
vector<float> FunctionFloatParametersBase(int functionIndex);