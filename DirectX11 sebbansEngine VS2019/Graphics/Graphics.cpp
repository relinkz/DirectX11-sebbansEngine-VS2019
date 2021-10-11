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
	float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	m_deviceContext->IASetInputLayout(m_vertexShader->GetInputLayout());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->VSSetShader(m_vertexShader->GetShader(), NULL, 0);
	m_deviceContext->RSSetState(m_rasterizerState.Get());
	m_deviceContext->PSSetShader(m_pixelShader->GetShader(), NULL, 0);
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf()); // see pixel shader register
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);


	// Update Constant buffer
	CB_VS_vertexShader cData;
	cData.xOffset = 0.0f;
	cData.yOffset = 0.5f;
	SetDynamicConstantBuffer(0, cData);

	for (size_t i = 0; i < m_vertexBuffer.size(); i++)
	{
		UINT offset = 0;
		UINT stride = m_vertexBuffer.at(i)->GetStride();

		m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.at(i)->GetBufferAddress(), &stride, &offset);
		m_deviceContext->IASetIndexBuffer(m_indexBuffers.at(i)->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
		
		m_deviceContext->DrawIndexed(m_indexBuffers.at(i)->GetNrOfIndencies(), 0, 0);
	}

	// Draw text
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Hello World", DirectX::XMFLOAT2(0,0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f,1.0f));
	m_spriteBatch->End();

	m_swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, const int width, const int height)
{
	if (!InitializeSwapChain(hwnd, width, height))
	{
		return false;
	}

	if (!InitializeRenderTargetViewWithSwapchain())
	{
		return false;
	}

	if (!InitializeDepthStencil(width, height))
	{
		return false;
	}

	if (!InitializeDepthStencilState())
	{
		return false;
	}

	if (!InitializeViewport(width, height))
	{
		return false;
	}

	if (!InitializeRasterizer())
	{
		return false;
	}

	if (!InitializeFonts())
	{
		return false;
	}

	if (!InitializeSamplerStates())
	{
		return false;
	}

	if (!InitializeConstantBuffers())
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeSwapChain(HWND hwnd, const int width, const int height)
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

bool Graphics::InitializeRenderTargetViewWithSwapchain()
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

bool Graphics::InitializeDepthStencil(const int width, const int height)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	auto hr = m_device->CreateTexture2D(&depthStencilDesc, NULL, m_depthStencilTexture.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create depth stencil texture buffer");
		return false;
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture.Get(), NULL, m_depthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create depth stencil view");
		return false;
	}

	// assign depthStencil to output merger
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	return true;
}

bool Graphics::InitializeDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	auto hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create depth stencil view");
		return false;
	}

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
	std::vector<Vertex> triangle = 
	{ 
		Vertex(), Vertex(), Vertex(), Vertex()
	};

	triangle[0].m_pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 1.0f); // Bot Left
	triangle[0].m_color = DirectX::XMFLOAT3(DirectX::Colors::Red);
	triangle[0].m_texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);

	triangle[1].m_pos = DirectX::XMFLOAT3(-0.5f, 0.5f, 1.0f); // Top Left
	triangle[1].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	triangle[1].m_texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);

	triangle[2].m_pos = DirectX::XMFLOAT3(0.5f, 0.5f, 1.0f); // Top Right
	triangle[2].m_color = DirectX::XMFLOAT3(DirectX::Colors::Blue);
	triangle[2].m_texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);

	triangle[3].m_pos = DirectX::XMFLOAT3(0.5f, -0.5f, 1.0f); // Bot Right
	triangle[3].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	triangle[3].m_texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	std::vector<DWORD> indices =
	{
		0, 1, 2,
		0, 2, 3
	};

	auto triangleBuff = resourceFactory.CreateSimpleVertexBuffer(m_device, triangle);
	if (!triangleBuff)
	{
		return false;
	}

	auto triIndBuff = resourceFactory.CreateSimpleIndexBuffer(m_device, indices);
	if (!triIndBuff)
	{
		return false;
	}

	m_vertexBuffer.push_back(std::move(triangleBuff));
	m_indexBuffers.push_back(std::move(triIndBuff));

	std::wstring pathToFile = L"Data\\Textures\\YaoMingMeme.jpg";
	if (InitializeTexture(pathToFile))
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeViewport(const int width, const int height)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// rasterizer
	m_deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeRasterizer()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	auto hr = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create rasterizer state");
		return false;
	}

	return true;
}

bool Graphics::InitializeFonts()
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());
	if (!m_spriteBatch)
	{
		errorlogger::Log("Failed to create SpriteBatch");
		return false;
	}

	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");
	if (!m_spriteFont)
	{
		errorlogger::Log("Failed to create SpriteFont: Path not found");
		return false;
	}

	return true;
}

bool Graphics::InitializeSamplerStates()
{
	D3D11_SAMPLER_DESC sampleDesc;
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));

	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	auto hr = m_device->CreateSamplerState(&sampleDesc, m_samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create sampler state");
		return false;
	}

	return true;
}

bool Graphics::InitializeTexture(const std::wstring& filePath)
{
	/*
	* need CoInitialize to be called before working, spritefont might be the one calling according to internet.
	*/
	auto hr = DirectX::CreateWICTextureFromFile(m_device.Get(), filePath.c_str(), nullptr, m_texture.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create Texture");
		return false;
	}

	return false;
}

bool Graphics::InitializeConstantBuffers()
{
	auto resourceFactory = ResourceBufferFactory();

	auto simpleConstantBuffer = resourceFactory.CreateSimpleConstantBuffer(m_device);
	if (!simpleConstantBuffer)
	{
		return false;
	}

	m_constantBuffers.push_back(std::move(simpleConstantBuffer));

	return true;
}

bool Graphics::SetDynamicConstantBuffer(const size_t index, CB_VS_vertexShader newData) const
{
	if (m_constantBuffers.size() < index)
	{
		errorlogger::Log("setDynamicConstantBuffer, index higher an vector size");
		return false;
	}

	// Update Constant buffer
	CB_VS_vertexShader cData;
	cData.xOffset = 0.0f;
	cData.yOffset = 0.5;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto hr = m_deviceContext->Map(m_constantBuffers.at(index)->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	CopyMemory(mappedResource.pData, &cData, sizeof(CB_VS_vertexShader));
	m_deviceContext->Unmap(m_constantBuffers.at(index)->GetBuffer(), index);
	m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffers.at(index)->GetBufferAddress());

	return false;
}
