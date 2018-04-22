#pragma once

#include "TextureMemory.h"


TextureMemoryPtr NormalColor(TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr HeightToNormal(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float height);