#include "SwapChain.h"

#include <vector>
#include "../ErrorLogger.h"
#include "AdapterReader.h"

namespace swapChain {

namespace {
	DXGI_SWAP_CHAIN_DESC createDesc(HWND hwnd,int windowWidth, int windowHeight) {
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferDesc.Width = windowWidth;
		scd.BufferDesc.Height = windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60; // Is only used on fullscreen
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Anti aliasing. this setting means NO ANTIALIASING
		scd.SampleDesc.Count = 1; // The number of multisamples per pixel.
		scd.SampleDesc.Quality = 0; // The image quality level. The higher the quality, the lower the performance.

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		return scd;
	}
}

Microsoft::WRL::ComPtr<IDXGISwapChain> newSwapChain(HWND hwnd, int windowWidth,
		int windowHeight,
		Microsoft::WRL::ComPtr<ID3D11Device>& device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) {
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		if (adapters.empty())
		{
			errorlogger::Log("No DXGI adapters (graphics card) found");
			COM_ERROR_IF_FAILED((HRESULT) - 1, "Failed to create swapchain.");
		}

		auto scd = createDesc(hwnd, windowWidth, windowHeight);
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapters.front().m_pAdapter,			// A pointer to the video adapter to use when creating a device
			D3D_DRIVER_TYPE_UNKNOWN,				// The D3D_DRIVER_TYPE, which represents the driver type to create.
			NULL,														// A handle to a DLL that implements a software rasterizer
			NULL,														// Runtime layers to enable
			NULL,														// D3D_feature_level, NULL means using an array of feature levels
			0,															// The number of feature levels (above), set to 0 since we are not specifing any numbers
			D3D11_SDK_VERSION,							// SDK version
			&scd,														// Swap chain description, contains init params to the swap chain
			swapChain.GetAddressOf(),			// IDXGISwapChain object that represents the swap chain used for rendering.
			device.GetAddressOf(),				// Device Address
			NULL,														// supported feature level
			deviceContext.GetAddressOf()	// Device context address
		);

		COM_ERROR_IF_FAILED(hr, "Failed to create swapchain.");
		return swapChain;
	}
}