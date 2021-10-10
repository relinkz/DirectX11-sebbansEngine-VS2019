#pragma once
/*
This class handles information on the graphicscard.
*/
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include "../ErrorLogger.h"
#include <wrl/client.h> //smart pointers for COM objects
#include <vector>

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* m_pAdapter;
	DXGI_ADAPTER_DESC m_desc;
};

class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();
private:
	static std::vector<AdapterData> m_adapters;
};