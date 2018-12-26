#include "stdafx.h"
#include "UtilityFunctions.h"


TextureMemoryPtr BackgroundGridTexture()
{
	TextureMemoryPtr backgroundGridTexturePtr = make_shared<TextureMemory>(GRAYSCALE, RES128, BPC8);

	for(int i = 0; i < RES128; i++)
	{
		for(int j = 0; j < RES128; j++)
		{
			if(i <= 1 || i >= 126 || j <= 1 || j >= 126)
			{
				backgroundGridTexturePtr->SetValue(i, j, 0.5f);
			}
			else if(i == 63 || i == 64 || j == 63 || j == 64)
			{
				backgroundGridTexturePtr->SetValue(i, j, 0.35f);
			}
			else
			{
				backgroundGridTexturePtr->SetValue(i, j, 0.25f);
			}
		}
	}

	return backgroundGridTexturePtr;
}


TextureMemoryPtr NotProcessedTexture()
{
	TextureMemoryPtr notProcessedTexturePtr = make_shared<TextureMemory>(GRAYSCALE, RES16, BPC8);

	for(int i = 0; i < RES16; i++)
	{
		for(int j = 0; j < RES16; j++)
		{
			notProcessedTexturePtr->SetValue(i, j, 0.0f);
		}
	}

	return notProcessedTexturePtr;
}
