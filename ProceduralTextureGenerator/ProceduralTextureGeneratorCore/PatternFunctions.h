#pragma once

#include "TextureMemory.h"


TextureMemoryPtr Shape(TextureResolution resolution, BitsPerChannel bitsPerChannel, int shapeIndex, int sidesNum);
TextureMemoryPtr LinearGradient(TextureResolution resolution, BitsPerChannel bitsPerChannel, float angle);
