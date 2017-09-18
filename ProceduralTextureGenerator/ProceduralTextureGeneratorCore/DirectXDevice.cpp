#include "stdafx.h"
#include "DirectXDevice.h"


DirectXDevice::DirectXDevice()
{
	driverType = D3D_DRIVER_TYPE_NULL;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	device = NULL;
	painter = NULL;
}


DirectXDevice::~DirectXDevice()
{

}


ID3D11Device *DirectXDevice::GetDevice()
{
	return device;
}


ID3D11DeviceContext *DirectXDevice::GetPainter()
{
	return painter;
}


HRESULT DirectXDevice::Init()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
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
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(0, driverType, 0, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &device, &featureLevel, &painter);
		if(SUCCEEDED(hr))
		{
			break;
		}
	}

	return hr;
}


void DirectXDevice::Release()
{
	if(painter) painter->ClearState();

	if(device) device->Release();
	if(painter) painter->Release();
}