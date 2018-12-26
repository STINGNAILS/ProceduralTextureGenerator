#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"


TextureMemoryPtr UniformColor(TextureResolution resolution, BitsPerChannel bitsPerChannel, TextureType textureType, XMFLOAT4 color);
TextureMemoryPtr MetalReflectance(TextureResolution resolution, BitsPerChannel bitsPerChannel, int metalIndex);