#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "MathHelper.h"

TextureMemoryPtr PerlinNoise(int size, int octaves, int gridStartingSize, float persistence);