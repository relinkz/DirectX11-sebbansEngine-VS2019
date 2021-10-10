#include "Graphics.h"
#include "ShaderFactory.h"
#include "ResourceBufferFactory.h"
#include "AdapterReader.h"


bool Graphics::Initialize(HWND hwnd, const int width, const int height)
{
	if (!InitializeDirectX(hwnd, width, height))
	{
		return false;
	}

	if (!InitializeShaders())
	{
		return false;
	}

	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

void Graphics::RenderFrame() const
{
	float bgColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
	m_deviceContext->IASetInputLayout(m_vertexShader->GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->VSSetShader(m_vertexShader->GetShader(), NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader->GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer->GetBufferAddress(), &stride, &offset);

	m_deviceContext->Draw(m_vertexBuffer->GetNrOfVerticies(), 0);

	m_swapchain->Present(1, NULL);
}

bool Graphics::CreateSwapChain(HWND hwnd, const int width, const int height)
{
	std::vector<AdapterData> adapter = AdapterReader::GetAdapters();

	if (adapter.empty())
	{
		errorlogger::Log("No DXGI adapters (graphics card) found");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		adapter.front().m_pAdapter,			// A pointer to the video adapter to use when creating a device
		D3D_DRIVER_TYPE_UNKNOWN,				// The D3D_DRIVER_TYPE, which represents the driver type to create.
		NULL,														// A handle to a DLL that implements a software rasterizer
		NULL,														// Runtime layers to enable
		NULL,														// D3D_feature_level, NULL means using an array of feature levels
		0,															// The number of feature levels (above), set to 0 since we are not specifing any numbers
		D3D11_SDK_VERSION,							// SDK version
		&scd,														// Swap chain description, contains init params to the swap chain
		m_swapchain.GetAddressOf(),			// IDXGISwapChain object that represents the swap chain used for rendering.
		m_device.GetAddressOf(),				// Device Address
		NULL,														// supported feature level
		m_deviceContext.GetAddressOf()	// Device context address
	);

	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create swapchain");
		return false;
	}

	return true;
}

bool Graphics::CreateRenderTargetViewWithSwapchain()
{
	// get backbuffer as a 2D texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to get texture from backbuffer");
		return false;
	}

	// create the render target view
	hr = m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create render target view");
		return false;
	}
	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, const int width, const int height)
{
	if (!CreateSwapChain(hwnd, width, height))
	{
		return false;
	}

	if (!CreateRenderTargetViewWithSwapchain())
	{
		return false;
	}

	// output merger
	ID3D11DepthStencilView* dsv = NULL;
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), dsv);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);

	// rasterizer
	m_deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeShaders()
{
	ShaderFactory shaderFactory = ShaderFactory();
	m_vertexShader = shaderFactory.CreateDefaultVertexShader(m_device);
	if (!m_vertexShader)
	{
		return false;
	}

	m_pixelShader = shaderFactory.CreateDefaultPixelShader(m_device);
	if (!m_pixelShader)
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeScene()
{
	ResourceBufferFactory resourceFactory = ResourceBufferFactory();
	m_vertexBuffer = resourceFactory.CreatePointVertexBuffer(m_device);
	if (!m_vertexBuffer)
	{
		return false;
	}
	return true;
}
