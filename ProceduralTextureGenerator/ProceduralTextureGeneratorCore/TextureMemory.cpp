#include "stdafx.h"
#include "TextureMemory.h"


TextureMemory::TextureMemory(TextureType textureType_, TextureResolution textureResolution_, BitsPerChannel bitsPerChannel_)
{
	textureType = textureType_;
	textureResolution = textureResolution_;
	bitsPerChannel = bitsPerChannel_;

	switch(bitsPerChannel)
	{
		case BPC8:
		{
			switch(textureType)
			{
				case GRAYSCALE:
				{
					textureMemory = new uint8_t[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(uint8_t));
					break;
				}
				case COLOR:
				{
					textureMemory = new XMUBYTEN4[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(XMUBYTEN4));
					break;
				}
			}
			break;
		}
		case BPC16:
		{
			switch(textureType)
			{
				case GRAYSCALE:
				{
					textureMemory = new uint16_t[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(uint16_t));
					break;
				}
				case COLOR:
				{
					textureMemory = new XMUSHORTN4[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(XMUSHORTN4));
					break;
				}
			}
			break;
		}
		case BPC32:
		{
			switch(textureType)
			{
				case GRAYSCALE:
				{
					textureMemory = new float[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(float));
					break;
				}
				case COLOR:
				{
					textureMemory = new XMFLOAT4[textureResolution * textureResolution];
					ZeroMemory(textureMemory, textureResolution * textureResolution * sizeof(XMFLOAT4));
					break;
				}
			}
			break;
		}
	}
}


TextureMemory::~TextureMemory()
{
	delete[] textureMemory;
}


inline float TextureMemory::SampleGrayscale8(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		XMUBYTEN2 value = XMUBYTEN2(((uint8_t*) textureMemory)[u * textureResolution + v], 0);

		XMFLOAT2 result;
		XMStoreFloat2(&result, XMLoadUByteN2(&value));

		return result.x;
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		XMVECTOR sumV = XMVectorZero();

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				XMUBYTEN2 value = XMUBYTEN2(((uint8_t*) textureMemory)[(ku + i) * textureResolution + (kv + j)], 0);

				sumV = XMVectorAdd(sumV, XMLoadUByteN2(&value));
			}
		}

		XMVECTOR divisorV = XMVectorReplicate(k * k);
		sumV = XMVectorDivide(sumV, divisorV);

		XMFLOAT2 sum;
		XMStoreFloat2(&sum, sumV);

		return sum.x;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;

		XMUBYTEN2 value = XMUBYTEN2(((uint8_t*) textureMemory)[(u / k) * textureResolution + (v / k)], 0);

		XMFLOAT2 result;
		XMStoreFloat2(&result, XMLoadUByteN2(&value));
		
		return result.x;
	}
}


inline float TextureMemory::SampleGrayscale16(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		XMUSHORTN2 value = XMUSHORTN2(((uint16_t*) textureMemory)[u * textureResolution + v], 0);

		XMFLOAT2 result;
		XMStoreFloat2(&result, XMLoadUShortN2(&value));

		return result.x;
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		XMVECTOR sumV = XMVectorZero();

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				XMUSHORTN2 value = XMUSHORTN2(((uint16_t*) textureMemory)[(ku + i) * textureResolution + (kv + j)], 0);

				sumV = XMVectorAdd(sumV, XMLoadUShortN2(&value));
			}
		}

		XMVECTOR divisorV = XMVectorReplicate(k * k);
		sumV = XMVectorDivide(sumV, divisorV);

		XMFLOAT2 sum;
		XMStoreFloat2(&sum, sumV);

		return sum.x;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;

		XMUSHORTN2 value = XMUSHORTN2(((uint16_t*) textureMemory)[(u / k) * textureResolution + (v / k)], 0);

		XMFLOAT2 result;
		XMStoreFloat2(&result, XMLoadUShortN2(&value));

		return result.x;
	}
}


inline float TextureMemory::SampleGrayscale32(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		return ((float*) textureMemory)[u * textureResolution + v];
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		float sum = 0.0f;

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				sum += ((float*) textureMemory)[(ku + i) * textureResolution + (kv + j)];
			}
		}

		sum /= k * k;

		return sum;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;
		return ((float*) textureMemory)[(u / k) * textureResolution + (v / k)];
	}
}


inline XMFLOAT4 TextureMemory::SampleColor8(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		XMFLOAT4 result;
		XMStoreFloat4(&result, XMLoadUByteN4(&((XMUBYTEN4*) textureMemory)[u * textureResolution + v]));

		return result;
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		XMVECTOR sumV = XMVectorZero();

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				sumV = XMVectorAdd(sumV, XMLoadUByteN4(&((XMUBYTEN4*) textureMemory)[(ku + i) * textureResolution + (kv + j)]));
			}
		}

		XMVECTOR divisorV = XMVectorReplicate(k * k);
		sumV = XMVectorDivide(sumV, divisorV);

		XMFLOAT4 sum;
		XMStoreFloat4(&sum, sumV);

		return sum;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;

		XMFLOAT4 result;
		XMStoreFloat4(&result, XMLoadUByteN4(&((XMUBYTEN4*) textureMemory)[(u / k) * textureResolution + (v / k)]));

		return result;
	}
}


inline XMFLOAT4 TextureMemory::SampleColor16(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		XMFLOAT4 result;
		XMStoreFloat4(&result, XMLoadUShortN4(&((XMUSHORTN4*) textureMemory)[u * textureResolution + v]));

		return result;
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		XMVECTOR sumV = XMVectorZero();

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				sumV = XMVectorAdd(sumV, XMLoadUShortN4(&((XMUSHORTN4*) textureMemory)[(ku + i) * textureResolution + (kv + j)]));
			}
		}

		XMVECTOR divisorV = XMVectorReplicate(k * k);
		sumV = XMVectorDivide(sumV, divisorV);

		XMFLOAT4 sum;
		XMStoreFloat4(&sum, sumV);

		return sum;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;

		XMFLOAT4 result;
		XMStoreFloat4(&result, XMLoadUShortN4(&((XMUSHORTN4*) textureMemory)[(u / k) * textureResolution + (v / k)]));

		return result;
	}
}


inline XMFLOAT4 TextureMemory::SampleColor32(UINT u, UINT v, TextureResolution sampleResolution)
{
	if(sampleResolution == textureResolution)
	{
		return ((XMFLOAT4*) textureMemory)[u * textureResolution + v];
	}
	else if(sampleResolution < textureResolution)
	{
		UINT k = textureResolution / sampleResolution;

		UINT ku = k * u;
		UINT kv = k * v;

		XMVECTOR sumV = XMVectorZero();

		for(UINT i = 0; i < k; i++)
		{
			for(UINT j = 0; j < k; j++)
			{
				sumV = XMVectorAdd(sumV, XMLoadFloat4(&((XMFLOAT4*) textureMemory)[(ku + i) * textureResolution + (kv + j)]));
			}
		}

		XMVECTOR divisorV = XMVectorReplicate(k * k);
		sumV = XMVectorDivide(sumV, divisorV);

		XMFLOAT4 sum;
		XMStoreFloat4(&sum, sumV);

		return sum;
	}
	else
	{
		UINT k = sampleResolution / textureResolution;
		return ((XMFLOAT4*) textureMemory)[(u / k) * textureResolution + (v / k)];
	}
}


TextureType TextureMemory::GetTextureType()
{
	return textureType;
}


TextureResolution TextureMemory::GetResolution()
{
	return textureResolution;
}


BitsPerChannel TextureMemory::GetFormat()
{
	return bitsPerChannel;
}


void *TextureMemory::GetMemoryPtr()
{
	return textureMemory;
}


float TextureMemory::SampleGrayscale(UINT u, UINT v, TextureResolution sampleResolution)
{
	switch(textureType)
	{
		case GRAYSCALE:
		{
			switch(bitsPerChannel)
			{
				case BPC8:
				{
					return SampleGrayscale8(u, v, sampleResolution);
				}
				case BPC16:
				{
					return SampleGrayscale16(u, v, sampleResolution);
				}
				case BPC32:
				{
					return SampleGrayscale32(u, v, sampleResolution);
				}
			}
		}
		case COLOR:
		{
			XMFLOAT4 temp;

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					temp = SampleColor8(u, v, sampleResolution);
					break;
				}
				case BPC16:
				{
					temp = SampleColor16(u, v, sampleResolution);
					break;
				}
				case BPC32:
				{
					temp = SampleColor32(u, v, sampleResolution);
					break;
				}
			}

			return (temp.x + temp.y + temp.z) / 3.0f;
		}
	}
}


XMFLOAT4 TextureMemory::SampleColor(UINT u, UINT v, TextureResolution sampleResolution)
{
	switch(textureType)
	{
		case GRAYSCALE:
		{
			float temp;

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					temp = SampleGrayscale8(u, v, sampleResolution);
					break;
				}
				case BPC16:
				{
					temp = SampleGrayscale16(u, v, sampleResolution);
					break;
				}
				case BPC32:
				{
					temp = SampleGrayscale32(u, v, sampleResolution);
					break;
				}
			}

			return XMFLOAT4(temp, temp, temp, 1.0f);
		}
		case COLOR:
		{
			switch(bitsPerChannel)
			{
				case BPC8:
				{
					return SampleColor8(u, v, sampleResolution);
				}
				case BPC16:
				{
					return SampleColor16(u, v, sampleResolution);
				}
				case BPC32:
				{
					return SampleColor32(u, v, sampleResolution);
				}
			}
		}
	}
}


void TextureMemory::SetValue(UINT u, UINT v, float value)
{
	switch(textureType)
	{
		case GRAYSCALE:
		{
			XMFLOAT2 value_ = XMFLOAT2(value, 1.0f);
			XMVECTOR tempV = XMVectorSaturate(XMLoadFloat2(&value_));

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					XMUBYTEN2 result;
					XMStoreUByteN2(&result, tempV);
					((uint8_t*) textureMemory)[u * textureResolution + v] = result.x;
					break;
				}
				case BPC16:
				{
					XMUSHORTN2 result;
					XMStoreUShortN2(&result, tempV);
					((uint16_t*) textureMemory)[u * textureResolution + v] = result.x;
					break;
				}
				case BPC32:
				{
					((float*) textureMemory)[u * textureResolution + v] = min(max(value, 0.0f), 1.0f);
					break;
				}
			}
			break;
		}
		case COLOR:
		{
			XMFLOAT4 value_ = XMFLOAT4(value, value, value, 1.0f);
			XMVECTOR tempV = XMVectorSaturate(XMLoadFloat4(&value_));

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					XMStoreUByteN4(&((XMUBYTEN4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
				case BPC16:
				{
					XMStoreUShortN4(&((XMUSHORTN4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
				case BPC32:
				{
					XMStoreFloat4(&((XMFLOAT4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
			}
			break;
		}
	}
}


void TextureMemory::SetValue(UINT u, UINT v, XMFLOAT4 value)
{
	switch(textureType)
	{
		case GRAYSCALE:
		{
			XMFLOAT2 value_ = XMFLOAT2((value.x + value.y + value.z) / 3.0f, 1.0f);
			XMVECTOR tempV = XMVectorSaturate(XMLoadFloat2(&value_));

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					XMUBYTEN2 result;
					XMStoreUByteN2(&result, tempV);
					((uint8_t*) textureMemory)[u * textureResolution + v] = result.x;
					break;
				}
				case BPC16:
				{
					XMUSHORTN2 result;
					XMStoreUShortN2(&result, tempV);
					((uint16_t*) textureMemory)[u * textureResolution + v] = result.x;
					break;
				}
				case BPC32:
				{
					((float*) textureMemory)[u * textureResolution + v] = min(max(value_.x, 0.0f), 1.0f);
					break;
				}
			}
			break;
		}
		case COLOR:
		{
			XMFLOAT4 value_ = XMFLOAT4(value.x, value.y, value.z, 1.0f);
			XMVECTOR tempV = XMVectorSaturate(XMLoadFloat4(&value_));

			switch(bitsPerChannel)
			{
				case BPC8:
				{
					XMStoreUByteN4(&((XMUBYTEN4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
				case BPC16:
				{
					XMStoreUShortN4(&((XMUSHORTN4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
				case BPC32:
				{
					XMStoreFloat4(&((XMFLOAT4*) textureMemory)[u * textureResolution + v], tempV);
					break;
				}
			}
			break;
		}
	}
}