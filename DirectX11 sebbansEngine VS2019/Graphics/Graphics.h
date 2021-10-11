#pragma once
#include "IShaders.h"
#include "IResourceBuffer.h"
#include <memory>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class Graphics
{
public:
	bool Initialize(HWND hwnd, const int width, const int height);
	void RenderFrame() const;
private:
	bool CreateSwapChain(HWND hwnd, const int width, const int height);
	bool CreateRenderTargetViewWithSwapchain();
	bool CreateDepthStencil(const int width, const int height);
	bool CreateDepthStencilState();
	bool InitializeDirectX(HWND hwnd, const int width, const int height);
	bool InitializeShaders();
	bool InitializeScene();
	bool InitializeViewport(const int width, const int height);
	bool InitializeRasterizer();
	bool InitializeFonts();
	bool InitializeSamplerStates();
	bool InitializeTexture(const std::wstring& filePath);

	// The device interface represents a virtual adapter; it is used to create resources.
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	// The ID3D11DeviceContext interface represents a device context which generates rendering commands.
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	// An IDXGISwapChain interface implements one or more surfaces for storing rendered data before presenting it to an output.
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapchain;
	// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	// Settings for the rasterizer, clock wise read order and wireframe
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	// The sampler-state interface holds a description for sampler state that you can bind to any shader stage of the pipeline for reference by texture sample operations.
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_myTexture;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	std::unique_ptr<IVertexShader> m_vertexShader;
	std::unique_ptr<IPixelShader> m_pixelShader;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	std::vector<std::unique_ptr<IResourceBuffer>> m_vertexBuffer;
	std::vector<std::unique_ptr<IResourceIndexBuffer>> m_indexBuffers;
};
