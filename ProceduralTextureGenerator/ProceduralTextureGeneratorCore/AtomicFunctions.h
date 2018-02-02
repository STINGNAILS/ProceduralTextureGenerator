#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "MathHelper.h"


TextureMemoryPtr BaseColor(TextureMemoryPtr inputTexturePtr, int size);
TextureMemoryPtr BaseGrayscale(TextureMemoryPtr inputTexturePtr, int size);
TextureMemoryPtr UniformColor(int size, TextureType textureType, XMFLOAT4 color);
TextureMemoryPtr Blend(TextureMemoryPtr foregroundTexturePtr, TextureMemoryPtr backgroundTexturePtr, TextureMemoryPtr blendCoefficientTexturePtr, int size, int blendMode, float k);
TextureMemoryPtr Levels(TextureMemoryPtr inputTexturePtr, int size, float x1, float x2, float x3, float x4, float x5);
TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, int size);
TextureMemoryPtr NormalColor(int size);