#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "ColorFunctions.h"


TextureMemoryPtr BaseColor(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Metallic(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Roughness(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Normal(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);