#include "stdafx.h"
#include "PatternFunctions.h"


TextureMemoryPtr Shape(TextureResolution resolution, BitsPerChannel bitsPerChannel, int shapeIndex, int sidesNum)
{
	TextureMemoryPtr shapeTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	switch(shapeIndex)
	{
		//Polygon
		case 0:
		{
			double segmentAngle = 2.0 * _Pi / sidesNum;

			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				double y = 1.0 - i * 2.0 / resolution;
				for(int j = 0; j < resolution; j++)
				{
					double x = j * 2.0 / resolution - 1.0;

					double angle = acos(x / sqrt(x * x + y * y));
					angle = y < 0.0f ? -angle : angle;

					int k = (int) round((1.5 * _Pi - angle) / segmentAngle);
					
					double angle0 = (1.5 * _Pi - (k - 0.5) * segmentAngle);
					double angle1 = (1.5 * _Pi - (k + 0.5) * segmentAngle);
					double x0 = cos(angle0);
					double y0 = sin(angle0);
					double x1 = cos(angle1);
					double y1 = sin(angle1);

					float value = (((y0 - y1) * x + (x1 - x0) * y + (x0 * y1 - x1 * y0) <= 0.0) ? 1.0f : 0.0f);
					shapeTexturePtr->SetValue(i, j, XMFLOAT2(value, 1.0f));
				}
			}
			break;
		}
		//Disc
		case 1:
		{
			#pragma omp parallel for
			for(int i = 0; i < resolution; i++)
			{
				double y = 1.0 - i * 2.0 / resolution;
				for(int j = 0; j < resolution; j++)
				{
					double x = j * 2.0 / resolution - 1.0;

					float value = x * x + y * y <= 1.0 ? 1.0f : 0.0f;
					shapeTexturePtr->SetValue(i, j, XMFLOAT2(value, 1.0f));
				}
			}
			break;
		}
	}

	return shapeTexturePtr;
}
