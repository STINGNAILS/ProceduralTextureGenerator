#include "stdafx.h"
#include "ColorFunctions.h"


TextureMemoryPtr UniformColor(TextureResolution resolution, BitsPerChannel bitsPerChannel, TextureType textureType, XMFLOAT4 color)
{
	TextureMemoryPtr uniformColorTexturePtr = make_shared<TextureMemory>(textureType, resolution, bitsPerChannel);

	switch(textureType)
	{
		case GRAYSCALE:
		{
			float color_ = color.x;

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					uniformColorTexturePtr->SetValue(i, j, color_);
				}
			}
			break;
		}
		case COLOR:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				for(int j = 0; j < resolution; j++)
				{
					uniformColorTexturePtr->SetValue(i, j, color);
				}
			}
			break;
		}
	}

	return uniformColorTexturePtr;
}


TextureMemoryPtr MetalReflectance(TextureResolution resolution, BitsPerChannel bitsPerChannel, int metalIndex)
{
	switch(metalIndex)
	{
		//Iron
		case 0:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.7686f, 0.7803f, 0.7803f, 1.0f));
		}
		//Titanium
		case 1:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.7568f, 0.7294f, 0.6941f, 1.0f));
		}
		//Alumimium
		case 2:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.9608f, 0.9647f, 0.9647f, 1.0f));
		}
		//Nickel
		case 3:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.8274f, 0.7960f, 0.7450f, 1.0f));
		}
		//Silver
		case 4:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.9882f, 0.9804f, 0.9608f, 1.0f));
		}
		//Gold
		case 5:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(1.0000f, 0.8863f, 0.6078f, 1.0f));
		}
		//Platinum
		case 6:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.8352f, 0.8156f, 0.7843f, 1.0f));
		}
		//Copper
		case 7:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.9803f, 0.8156f, 0.7529f, 1.0f));
		}
		//Chromium
		case 8:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.7621f, 0.7658f, 0.7646f, 1.0f));
		}
		//Cobalt
		case 9:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.8274f, 0.8235f, 0.8117f, 1.0f));
		}
		default:
		{
			return UniformColor(resolution, bitsPerChannel, COLOR, XMFLOAT4(0.0000f, 0.0000f, 0.0000f, 1.0f));
		}
	}
}