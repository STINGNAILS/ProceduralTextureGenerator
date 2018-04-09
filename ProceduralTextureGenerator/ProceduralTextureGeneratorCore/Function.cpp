#include "stdafx.h"
#include "Function.h"


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters)
{
	switch(functionIndex)
	{
		case BASE_COLOR:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return BaseColor(inputTexturePtr, resolution, bpc);
		}
		case METALLIC:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return Metallic(inputTexturePtr, resolution, bpc);
		}
		case ROUGHNESS:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return Roughness(inputTexturePtr, resolution, bpc);
		}
		case NORMAL:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return Normal(inputTexturePtr, resolution, bpc);
		}
		case HEIGHT:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return Height(inputTexturePtr, resolution, bpc);
		}
		case UNIFORM_COLOR:
		{
			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			TextureType textureType = (TextureType) intParameters[2];
			XMFLOAT4 color = XMFLOAT4(floatParameters[0], floatParameters[1], floatParameters[2], floatParameters[3]);

			return UniformColor(resolution, bpc, textureType, color);
		}
		case BLEND:
		{
			TextureMemoryPtr foregroundTexturePtr = inputTexturePtrs[0];
			TextureMemoryPtr backgroundTexturePtr = inputTexturePtrs[1];
			TextureMemoryPtr blendCoefficientTexturePtr = inputTexturePtrs[2];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			UINT blendMode = intParameters[2];
			float k = floatParameters[0];

			return Blend(foregroundTexturePtr, backgroundTexturePtr, blendCoefficientTexturePtr, resolution, bpc, blendMode, k);
		}
		case REMAP:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			UINT remapMode = intParameters[2];
			float x1 = floatParameters[0];
			float y1 = floatParameters[1];
			float x2 = floatParameters[2];
			float y2 = floatParameters[3];
			float x3 = floatParameters[4];
			float y3 = floatParameters[5];

			return Remap(inputTexturePtr, resolution, bpc, remapMode, x1, y1, x2, y2, x3, y3);
		}
		case GRADIENT:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return Gradient(inputTexturePtr, resolution, bpc);
		}
		case PERLIN_NOISE:
		{
			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			int seed = intParameters[2];
			int minimalOctave = intParameters[3];
			int maximalOctave = intParameters[4];
			float persistence = floatParameters[0];

			return PerlinNoise(resolution, bpc, seed, persistence, minimalOctave, maximalOctave);
		}
		case WORLEY_NOISE:
		{
			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			int seed = intParameters[2];
			int sitesNum = intParameters[3];
			int patternType = intParameters[4];
			int distanceType = intParameters[5];
			float borderWidth = floatParameters[0];

			return WorleyNoise(resolution, bpc, seed, sitesNum, patternType, distanceType, borderWidth);
		}
		case NORMAL_COLOR:
		{
			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];

			return NormalColor(resolution, bpc);
		}
		case BLUR:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			float intensity = floatParameters[0];

			return Blur(inputTexturePtr, resolution, bpc, intensity);
		}
		case DIRECTIONAL_BLUR:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			float intensity = floatParameters[0];
			float angle = floatParameters[1];

			return DirectionalBlur(inputTexturePtr, resolution, bpc, intensity, angle);
		}
		case METAL_REFLECTANCE:
		{
			TextureResolution resolution = (TextureResolution) intParameters[0];
			BitsPerChannel bpc = (BitsPerChannel) intParameters[1];
			int metalIndex = intParameters[2];

			return MetalReflectance(resolution, bpc, metalIndex);
		}
		default:
		{
			return nullptr;
		}
	}
}


int FunctionInputNodesNum(int functionIndex)
{
	switch(functionIndex)
	{
		case BASE_COLOR:
		{
			return 1;
		}
		case METALLIC:
		{
			return 1;
		}
		case ROUGHNESS:
		{
			return 1;
		}
		case NORMAL:
		{
			return 1;
		}
		case HEIGHT:
		{
			return 1;
		}
		case UNIFORM_COLOR:
		{
			return 0;
		}
		case BLEND:
		{
			return 3;
		}
		case REMAP:
		{
			return 1;
		}
		case GRADIENT:
		{
			return 1;
		}
		case PERLIN_NOISE:
		{
			return 0;
		}
		case WORLEY_NOISE:
		{
			return 0;
		}
		case NORMAL_COLOR:
		{
			return 0;
		}
		case BLUR:
		{
			return 1;
		}
		case DIRECTIONAL_BLUR:
		{
			return 1;
		}
		case METAL_REFLECTANCE:
		{
			return 0;
		}
		default:
		{
			return 0;
		}
	}
}


vector<int> FunctionIntParametersBase(int functionIndex)
{
	switch(functionIndex)
	{
		case BASE_COLOR:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case METALLIC:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case ROUGHNESS:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case NORMAL:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case HEIGHT:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case UNIFORM_COLOR:
		{
			vector<int> intParameters(3);
			intParameters[0] = 1024;
			intParameters[1] = 8;
			intParameters[2] = 1;

			return intParameters;
		}
		case BLEND:
		{
			vector<int> intParameters(3);
			intParameters[0] = 1024;
			intParameters[1] = 8;
			intParameters[2] = 0;

			return intParameters;
		}
		case REMAP:
		{
			vector<int> intParameters(3);
			intParameters[0] = 1024;
			intParameters[1] = 8;
			intParameters[2] = 1;

			return intParameters;
		}
		case GRADIENT:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 16;

			return intParameters;
		}
		case PERLIN_NOISE:
		{
			vector<int> intParameters(5);
			intParameters[0] = 1024;
			intParameters[1] = 16;
			intParameters[2] = 0;
			intParameters[3] = 1;
			intParameters[4] = 8;

			return intParameters;
		}
		case WORLEY_NOISE:
		{
			vector<int> intParameters(6);
			intParameters[0] = 1024;
			intParameters[1] = 16;
			intParameters[2] = 0;
			intParameters[3] = 50;
			intParameters[4] = 1;
			intParameters[5] = 2;

			return intParameters;
		}
		case NORMAL_COLOR:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case BLUR:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case DIRECTIONAL_BLUR:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
		case METAL_REFLECTANCE:
		{
			vector<int> intParameters(3);
			intParameters[0] = 1024;
			intParameters[1] = 8;
			intParameters[2] = 0;

			return intParameters;
		}
		default:
		{
			vector<int> intParameters(2);
			intParameters[0] = 1024;
			intParameters[1] = 8;

			return intParameters;
		}
	}
}


vector<float> FunctionFloatParametersBase(int functionIndex)
{
	switch(functionIndex)
	{
		case BASE_COLOR:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case METALLIC:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case ROUGHNESS:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case NORMAL:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case HEIGHT:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case UNIFORM_COLOR:
		{
			vector<float> floatParameters(4);
			floatParameters[0] = 0.0f;
			floatParameters[1] = 0.0f;
			floatParameters[2] = 0.0f;
			floatParameters[3] = 1.0f;

			return floatParameters;
		}
		case BLEND:
		{
			vector<float> floatParameters(1);
			floatParameters[0] = 1.0f;

			return floatParameters;
		}
		case REMAP:
		{
			vector<float> floatParameters(6);
			floatParameters[0] = 0.0f;
			floatParameters[1] = 0.0f;
			floatParameters[2] = 0.5f;
			floatParameters[3] = 0.5f;
			floatParameters[4] = 1.0f;
			floatParameters[5] = 1.0f;

			return floatParameters;
		}
		case GRADIENT:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case PERLIN_NOISE:
		{
			vector<float> floatParameters(1);
			floatParameters[0] = 0.5f;

			return floatParameters;
		}
		case WORLEY_NOISE:
		{
			vector<float> floatParameters(1);
			floatParameters[0] = 0.02f;

			return floatParameters;
		}
		case NORMAL_COLOR:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		case BLUR:
		{
			vector<float> floatParameters(1);
			floatParameters[0] = 1.0f;

			return floatParameters;
		}
		case DIRECTIONAL_BLUR:
		{
			vector<float> floatParameters(2);
			floatParameters[0] = 1.0f;
			floatParameters[1] = 0.0f;

			return floatParameters;
		}
		case METAL_REFLECTANCE:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
		default:
		{
			vector<float> floatParameters(0);

			return floatParameters;
		}
	}
}
