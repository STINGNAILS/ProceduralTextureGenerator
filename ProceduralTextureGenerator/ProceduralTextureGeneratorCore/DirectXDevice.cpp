#include "stdafx.h"
#include "DirectXDevice.h"


bool DirectXDevice::isInitialized = false;

ID3D11Device *DirectXDevice::device = nullptr;
ID3D11DeviceContext *DirectXDevice::painter = nullptr;


DirectXDevice::DirectXDevice()
{

}


DirectXDevice::~DirectXDevice()
{
	
}


HRESULT DirectXDevice::Init()
{
	HRESULT hr = S_OK;

	if(!isInitialized)
	{
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

			hr = D3D11CreateDevice(0, driverType, 0, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &device, &featureLevel, &painter);
			if(SUCCEEDED(hr))
			{
				isInitialized = true;
				break;
			}
		}
	}

	return hr;
}


bool DirectXDevice::IsInitialized()
{
	return isInitialized;
}


void DirectXDevice::Release()
{
	if(painter) painter->ClearState();

	if(painter) painter->Release();
	if(device) device->Release();

	isInitialized = false;
}


ID3D11Device *DirectXDevice::GetDevice()
{
	return device;
}


ID3D11DeviceContext *DirectXDevice::GetPainter()
{
	return painter;
}