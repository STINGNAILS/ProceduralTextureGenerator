#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "MathHelper.h"

TextureMemoryPtr PerlinNoise(TextureResolution resolution, BitsPerChannel bitsPerChannel, int octaves, int gridStartingSize, float persistence);