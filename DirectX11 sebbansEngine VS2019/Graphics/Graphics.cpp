#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, const int width, const int height)
{
	return InitializeDirectX(hwnd, width, height);
}

bool Graphics::InitializeDirectX(HWND hwnd, const int width, const int height)
{
	std::vector<AdapterData> adapter = AdapterReader::GetAdapters();

	return !adapter.empty();
}
