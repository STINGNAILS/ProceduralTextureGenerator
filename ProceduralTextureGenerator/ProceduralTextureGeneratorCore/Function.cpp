#include "stdafx.h"
#include "Function.h"


TextureMemoryPtr Function(int functionIndex, vector<TextureMemoryPtr> inputTexturePtrs, vector<int> intParameters, vector<float> floatParameters)
{
	switch(functionIndex)
	{
		case BASE_COLOR:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			int size = intParameters[0];

			return BaseColor(inputTexturePtr, size);
		}
		case BASE_GRAYSCALE:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			int size = intParameters[0];

			return BaseGrayscale(inputTexturePtr, size);
		}
		case UNIFORM_COLOR:
		{
			int size = intParameters[0];
			TextureType textureType = intParameters[1] == 0 ? GRAYSCALE : COLOR;
			XMFLOAT4 color = XMFLOAT4(floatParameters[0], floatParameters[1], floatParameters[2], floatParameters[3]);

			return UniformColor(size, textureType, color);
		}
		case BLEND:
		{
			TextureMemoryPtr foregroundTexturePtr = inputTexturePtrs[0];
			TextureMemoryPtr backgroundTexturePtr = inputTexturePtrs[1];
			TextureMemoryPtr blendCoefficientTexturePtr = inputTexturePtrs[2];

			int size = intParameters[0];
			int blendMode = intParameters[1];
			float k = floatParameters[0];

			return Blend(foregroundTexturePtr, backgroundTexturePtr, blendCoefficientTexturePtr, size, blendMode, k);
		}
		case LEVELS:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			int size = intParameters[0];
			float x1 = floatParameters[0];
			float x2 = floatParameters[1];
			float x3 = floatParameters[2];
			float x4 = floatParameters[3];
			float x5 = floatParameters[4];

			return Levels(inputTexturePtr, size, x1, x2, x3, x4, x5);
		}
		case GRADIENT:
		{
			TextureMemoryPtr inputTexturePtr = inputTexturePtrs[0];

			int size = intParameters[0];

			return Gradient(inputTexturePtr, size);
		}
		case PERLIN_NOISE:
		{
			int size = intParameters[0];
			int octaves = intParameters[1];
			int gridStartingSize = intParameters[2];
			float persistence = floatParameters[0];

			return PerlinNoise(size, octaves, gridStartingSize, persistence);
		}
		case NORMAL_COLOR:
		{
			int size = intParameters[0];

			return NormalColor(size);
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
		case BASE_GRAYSCALE:
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
		case LEVELS:
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
		case NORMAL_COLOR:
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
			vector<int> intParameters(1);
			intParameters[0] = 2048;

			return intParameters;
		}
		case BASE_GRAYSCALE:
		{
			vector<int> intParameters(1);
			intParameters[0] = 2048;

			return intParameters;
		}
		case UNIFORM_COLOR:
		{
			vector<int> intParameters(2);
			intParameters[0] = 2048;
			intParameters[1] = 1;

			return intParameters;
		}
		case BLEND:
		{
			vector<int> intParameters(2);
			intParameters[0] = 2048;
			intParameters[1] = 0;

			return intParameters;
		}
		case LEVELS:
		{
			vector<int> intParameters(1);
			intParameters[0] = 2048;

			return intParameters;
		}
		case GRADIENT:
		{
			vector<int> intParameters(1);
			intParameters[0] = 2048;

			return intParameters;
		}
		case PERLIN_NOISE:
		{
			vector<int> intParameters(3);
			intParameters[0] = 2048;
			intParameters[1] = 8;
			intParameters[2] = 2;

			return intParameters;
		}
		case NORMAL_COLOR:
		{
			vector<int> intParameters(1);
			intParameters[0] = 2048;

			return intParameters;
		}
		default:
		{
			vector<int> intParameters(1);
			intParameters[0] = 2048;

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
		case BASE_GRAYSCALE:
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
		case LEVELS:
		{
			vector<float> floatParameters(5);
			floatParameters[0] = 0.0f;
			floatParameters[1] = 0.5f;
			floatParameters[2] = 1.0f;
			floatParameters[3] = 0.0f;
			floatParameters[4] = 1.0f;

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
		case NORMAL_COLOR:
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
