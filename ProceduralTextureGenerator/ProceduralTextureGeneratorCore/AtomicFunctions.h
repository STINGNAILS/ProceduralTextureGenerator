#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "MathHelper.h"


TextureMemoryPtr BaseColor(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Metallic(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Roughness(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Normal(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Height(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr UniformColor(TextureResolution resolution, BitsPerChannel bitsPerChannel, TextureType textureType, XMFLOAT4 color);
TextureMemoryPtr Blend(TextureMemoryPtr foregroundTexturePtr, TextureMemoryPtr backgroundTexturePtr, TextureMemoryPtr blendCoefficientTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT blendMode, float k);
TextureMemoryPtr Remap(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT remapMode, float x1, float y1, float x2, float y2, float x3, float y3);
TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr NormalColor(TextureResolution resolution, BitsPerChannel bitsPerChannel);
TextureMemoryPtr Blur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity);
TextureMemoryPtr DirectionalBlur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, float angle);