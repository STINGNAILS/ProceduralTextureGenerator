#include "stdafx.h"
#include "ShaderCompiler.h"


HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined (_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob *errorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &errorBlob);
	if(FAILED(hr))
	{
		if(errorBlob != NULL)
		{
			OutputDebugStringA((char*) errorBlob->GetBufferPointer());
		}
		if(errorBlob)
		{
			errorBlob->Release();
		}
		return hr;
	}
	if(errorBlob)
	{
		errorBlob->Release();
	}

	return S_OK;
}