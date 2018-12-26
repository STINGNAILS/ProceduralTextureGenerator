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
					shapeTexturePtr->SetValue(i, j, value);
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
					shapeTexturePtr->SetValue(i, j, value);
				}
			}
			break;
		}
	}

	return shapeTexturePtr;
}


TextureMemoryPtr LinearGradient(TextureResolution resolution, BitsPerChannel bitsPerChannel, float angle)
{
	TextureMemoryPtr linearGradientTexturePtr = make_shared<TextureMemory>(GRAYSCALE, resolution, bitsPerChannel);

	double scale = 1.414214f * cos((45.0f - fmodf(angle, 90.0f)) * _Pi / 180.0f);
	double px = scale * cos((angle + 90.0f) * _Pi / 180.0f);
	double py = scale * sin((angle + 90.0f) * _Pi / 180.0f);

	double a = -py;
	double b = px;

	#pragma omp parallel for
	for(int i = 0; i < resolution; i++)
	{
		double y = 1.0 - i * 2.0 / resolution;
		for(int j = 0; j < resolution; j++)
		{
			double x = j * 2.0 / resolution - 1.0;

			double k;

			if(abs(a) < 1e-6f)
			{
				k = (x + px) / (2 * px);
			}
			else if(abs(b) < 1e-6f)
			{
				k = (y + py) / (2 * py);
			}
			else
			{
				double d = -b * x + a * y;
				k = (-(b * d) / (a * a + b * b) + px) / (2 * px);
			}

			linearGradientTexturePtr->SetValue(i, j, k);
		}
	}

	return linearGradientTexturePtr;
}
