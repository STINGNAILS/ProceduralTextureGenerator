#pragma once

#include "BaseFunctions.h"
#include "ColorFunctions.h"
#include "FilterFunctions.h"
#include "NoiseFunctions.h"
#include "PatternFunctions.h"
#include "NormalFunctions.h"
#include "TextureMemory.h"


#define BASE_COLOR 0
#define METALLIC 1
#define ROUGHNESS 2
#define NORMAL 3
//#define HEIGHT 4
#define UNIFORM_COLOR 5
#define BLEND 6
#define REMAP 7
#define GRADIENT 8
#define PERLIN_NOISE 9
#define WORLEY_NOISE 10
#define NORMAL_COLOR 11
#define BLUR 12
#define DIRECTIONAL_BLUR 13
#define METAL_REFLECTANCE 14
#define HEIGHT_TO_NORMAL 15
#define SHAPE 16
#define TRANSFORM 17
#define WARP 18
#define SLOPE_BLUR 19
#define LINEAR_GRADIENT 20


struct InputSlotDescriptor
{
	TextureType textureType;
	bool isMandatory;
};


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters);

vector<InputSlotDescriptor> FunctionInputSlotDescriptors(int functionIndex);
TextureType FunctionOutputSlotTextureType(int functionIndex);

vector<int> FunctionIntParametersBase(int functionIndex);
vector<float> FunctionFloatParametersBase(int functionIndex);