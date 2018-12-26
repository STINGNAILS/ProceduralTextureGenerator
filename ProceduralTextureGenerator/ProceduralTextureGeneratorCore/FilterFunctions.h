#pragma once

#include "TextureMemory.h"
#include "FunctionNode.h"
#include "ColorFunctions.h"


TextureMemoryPtr Blend(TextureMemoryPtr foregroundTexturePtr, TextureMemoryPtr backgroundTexturePtr, TextureMemoryPtr blendCoefficientTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT blendMode, float k);
TextureMemoryPtr Remap(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, UINT remapMode, float x1, float y1, float x2, float y2, float x3, float y3);
TextureMemoryPtr Gradient(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, int gradientSize, vector<float> gradient);
TextureMemoryPtr Blur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity);
TextureMemoryPtr DirectionalBlur(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, float angle);
TextureMemoryPtr Transform(TextureMemoryPtr inputTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, int tilingMode, float xScale, float yScale, float rotation, float xTranslate, float yTranslate);
TextureMemoryPtr Warp(TextureMemoryPtr inputTexturePtr, TextureMemoryPtr slopeTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity);
TextureMemoryPtr SlopeBlur(TextureMemoryPtr inputTexturePtr, TextureMemoryPtr slopeTexturePtr, TextureResolution resolution, BitsPerChannel bitsPerChannel, float intensity, int samplesNum);