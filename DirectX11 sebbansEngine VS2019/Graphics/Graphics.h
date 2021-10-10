#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <memory>

class Graphics
{
public:
	bool Initialize(HWND hwnd, const int width, const int height);
	void RenderFrame() const;
private:
	bool CreateSwapChain(HWND hwnd, const int width, const int height);
	bool CreateRenderTargetViewWithSwapchain();
	bool InitializeDirectX(HWND hwnd, const int width, const int height);
	bool InitializeShaders();

	// The device interface represents a virtual adapter; it is used to create resources.
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	// The ID3D11DeviceContext interface represents a device context which generates rendering commands.
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	// An IDXGISwapChain interface implements one or more surfaces for storing rendered data before presenting it to an output.
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	std::unique_ptr<VertexShader> m_vertexShader;
};
