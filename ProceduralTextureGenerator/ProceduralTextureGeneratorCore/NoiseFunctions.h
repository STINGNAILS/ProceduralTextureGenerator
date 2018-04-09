#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "MathHelper.h"


TextureMemoryPtr PerlinNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int seed, float persistence, int minimalOctave, int maximalOctave);
TextureMemoryPtr WorleyNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int seed, int sitesNum, int patternType, int distanceType, float borderWidth);