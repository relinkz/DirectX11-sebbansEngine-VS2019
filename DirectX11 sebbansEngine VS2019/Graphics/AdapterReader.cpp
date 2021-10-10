#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::m_adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter) :
	m_pAdapter(nullptr),
	m_desc()
{
	m_pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&m_desc);

	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to get description of IDXGIAdapter");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	if (!m_adapters.empty())
	{
		return m_adapters;
	}

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters");
		exit(-1);
	}

	IDXGIAdapter* pAdapter = nullptr;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		m_adapters.push_back(AdapterData(pAdapter));
		index++;
	}
	return m_adapters;
}