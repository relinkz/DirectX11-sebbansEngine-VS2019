#pragma once
#include "IShaders.h"
#include "IResourceBuffer.h"
#include "Camera.h"
#include "../Timer.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include <memory>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "IModel.h"


class Graphics
{
public:
	bool Initialize(HWND hwnd, const int width, const int height);
	void RenderFrame();
	std::unique_ptr<Camera> gameCamera;

	~Graphics();
private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeRenderTargetViewWithSwapchain();
	bool InitializeDepthStencil();
	bool InitializeDepthStencilState();
	bool InitializeScene();
	bool InitializeViewport();
	bool InitializeRasterizer();
	bool InitializeBlendState();
	bool InitializeFonts();
	bool InitializeSamplerStates();
	bool InitializeConstantBuffers();

	bool UpdateDynamicVsConstantBuffer(const size_t index, CB_VS_vertexShader newData) const;
	bool UpdateDynamicPsConstantBuffer(const size_t index, CB_PS_pixelAlphaShader newData) const;
	bool UpdateDynamicPsConstantBuffer(const size_t index, CB_PS_pixelMaterialShader newData) const;
	void InitializeImGui(HWND hwnd) const;
	void DestroyImGui() const;

	void UpdateCameraCB() const;
	void CreateGroundQuads();

	void PreparePipeline() const;
	void RenderImGui() const;
	void StartRender();

	// The device interface represents a virtual adapter; it is used to create resources.
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	// The ID3D11DeviceContext interface represents a device context which generates rendering commands.
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	
	// An IDXGISwapChain interface implements one or more surfaces for storing rendered data before presenting it to an output.
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

		// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	// Settings for the rasterizer, clock wise read order and wireframe
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerStateCullFront;
	// The sampler-state interface holds a description for sampler state that you can bind to any shader stage of the pipeline for reference by texture sample operations.
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_grassDiffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_grassNormalTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_occlusionTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularTexture;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	std::unique_ptr<IVertexShader> m_vertexShader;
	std::unique_ptr<IPixelShader> m_pixelShader;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<Timer> m_fpsTimer;

	// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	std::vector<std::unique_ptr<IResourceVertexBuffer>> m_vertexBuffer;
	std::vector<std::unique_ptr<IResourceIndexBuffer>> m_indexBuffers;
	std::vector<std::unique_ptr<IResourceConstantBuffer>> m_vsConstantBuffers;
	std::vector<std::unique_ptr<IResourceConstantBuffer>> m_psConstantBuffers;

	std::vector<std::unique_ptr<IModel>> m_modelsInScene;

	int m_windowWidth = 0;
	int m_windowHeight = 0;
};
