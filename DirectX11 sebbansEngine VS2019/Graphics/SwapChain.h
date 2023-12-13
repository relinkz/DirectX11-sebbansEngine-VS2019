#pragma once
#include <wrl/client.h>
#include <d3d11.h>

namespace swapChain {
	Microsoft::WRL::ComPtr<IDXGISwapChain> newSwapChain(HWND hwnd, int windowWidth,
		int windowHeight,
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext);
}