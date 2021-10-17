#include "Graphics.h"
#include "ShaderFactory.h"
#include "ResourceBufferFactory.h"
#include "AdapterReader.h"


bool Graphics::Initialize(HWND hwnd, const int width, const int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	try
	{
		if (!InitializeDirectX(hwnd))
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
	}
	catch (COMException& exception)
	{
		errorlogger::Log(exception);
		return false;
	}


	InitializeImGui(hwnd);


	gameCamera = std::make_unique<Camera>();
	gameCamera->SetPosition(0.0f, 0.0f, -2.0f);

	float rotationDegrees = 90.0f;
	float aspectRatio = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	gameCamera->SetProjectionValues(90.0f, aspectRatio, nearZ, farZ);

	m_fpsTimer = std::make_unique<Timer>();
	m_fpsTimer->Start();

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
	m_deviceContext->PSSetShader(m_pixelShader->GetShader(), NULL, 0);
	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf()); // see pixel shader register
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	m_deviceContext->OMSetBlendState(m_blendState.Get(), NULL, 0xFFFFFFFF);

	static float alpha = 1.0f;
	{ // pavement
		// Model to world matrix
		static float translationOffset[3] = { 0.0f, 0.0f, -1.0f };
		static float rotationOffset[3] = { 0.0f, 0.0f, 0.0f };

		auto rotationMatrix = XMMatrixRotationRollPitchYaw(rotationOffset[0], rotationOffset[1], rotationOffset[2]);
		auto scaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		auto translationMatrix = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
		DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

		CB_VS_vertexShader cData;
		cData.m_matrix = worldMatrix * gameCamera->GetViewMatrix() * gameCamera->GetProjectionMatrix();
		cData.m_matrix = DirectX::XMMatrixTranspose(cData.m_matrix);

		CB_PS_pixelShader cPsData;
		cPsData.alpha = alpha;

		UpdateDynamicVsConstantBuffer(0, cData);
		UpdateDynamicPsConstantBuffer(0, cPsData);

		for (size_t i = 0; i < m_vertexBuffer.size(); i++)
		{
			UINT offset = 0;
			UINT stride = m_vertexBuffer.at(i)->GetStride();

			m_deviceContext->PSSetShaderResources(0, 1, m_pavementTexture.GetAddressOf());
			m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.at(i)->GetBufferAddress(), &stride, &offset);
			m_deviceContext->IASetIndexBuffer(m_indexBuffers.at(i)->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
			
			m_deviceContext->RSSetState(m_rasterizerStateCullFront.Get());
			m_deviceContext->DrawIndexed(m_indexBuffers.at(i)->GetNrOfIndencies(), 0, 0);
			
			m_deviceContext->RSSetState(m_rasterizerState.Get());
			m_deviceContext->DrawIndexed(m_indexBuffers.at(i)->GetNrOfIndencies(), 0, 0);
		}
	}


	// Draw text
	static unsigned int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter++;
	constexpr float secondInMiliSec = 1000.0f;
	if (m_fpsTimer->GetMilisecondsElapsed() > secondInMiliSec)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		m_fpsTimer->Restart();
	}

	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), helpers::strings::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0,0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f,1.0f));
	m_spriteBatch->End();

	// start the imgui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static unsigned int counter = 0;

	// create test window
	ImGui::Begin("Object transform");
	ImGui::DragFloat("Alpha:", &alpha, 0.01f, 0, 1.0f);
	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_swapchain->Present(0, NULL);
}

Graphics::~Graphics()
{
	DestroyImGui();
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	if (!InitializeSwapChain(hwnd))
	{
		return false;
	}

	if (!InitializeRenderTargetViewWithSwapchain())
	{
		return false;
	}

	if (!InitializeDepthStencil())
	{
		return false;
	}

	if (!InitializeDepthStencilState())
	{
		return false;
	}

	if (!InitializeViewport())
	{
		return false;
	}

	if (!InitializeRasterizer())
	{
		return false;
	}

	if (!InitializeBlendState())
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

bool Graphics::InitializeSwapChain(HWND hwnd)
{
	std::vector<AdapterData> adapter = AdapterReader::GetAdapters();

	if (adapter.empty())
	{
		errorlogger::Log("No DXGI adapters (graphics card) found");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferDesc.Width = m_windowWidth;
	scd.BufferDesc.Height = m_windowHeight;
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

	COM_ERROR_IF_FAILED(hr, "Failed to create swapchain.");

	return true;
}

bool Graphics::InitializeRenderTargetViewWithSwapchain()
{
	// get backbuffer as a 2D texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to get texture from backbuffer.");


	// create the render target view
	hr = m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

	return true;
}

bool Graphics::InitializeDepthStencil()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_windowWidth;
	depthStencilDesc.Height = m_windowHeight;
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
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil texture buffer.");

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture.Get(), NULL, m_depthStencilView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

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
	COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil State.");

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
		Vertex(), Vertex(), Vertex(), Vertex(),
		Vertex(), Vertex(), Vertex(), Vertex()
	};

	triangle[0].m_pos = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f); // Front Bot Left
	triangle[1].m_pos = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f);	// Front Top Left
	triangle[2].m_pos = DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f);		// Front Top Right
	triangle[3].m_pos = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);	// Front Bot Right

	triangle[4].m_pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);	// Back Bot Left
	triangle[5].m_pos = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);		// Back Top Left
	triangle[6].m_pos = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);		// Back Top Right
	triangle[7].m_pos = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);		// Back Bot Right
	
	triangle[0].m_color = DirectX::XMFLOAT3(DirectX::Colors::Red);
	triangle[1].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	triangle[2].m_color = DirectX::XMFLOAT3(DirectX::Colors::Blue);
	triangle[3].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	triangle[4].m_color = DirectX::XMFLOAT3(DirectX::Colors::Red);
	triangle[5].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	triangle[6].m_color = DirectX::XMFLOAT3(DirectX::Colors::Blue);
	triangle[7].m_color = DirectX::XMFLOAT3(DirectX::Colors::Green);
	
	triangle[0].m_texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
	triangle[1].m_texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	triangle[2].m_texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
	triangle[3].m_texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);
	triangle[4].m_texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
	triangle[5].m_texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
	triangle[6].m_texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
	triangle[7].m_texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

	std::vector<DWORD> indices =
	{
		0, 1, 2, // Front
		0, 2, 3, // Front
		4, 7, 6, // Back
		4, 6, 5, // Back
		3, 2, 6, // Right side
		3, 6, 7, // Right side
		4, 5, 1, // Left side
		4, 1, 0, // Left side
		1, 5, 6, // Top
		1, 6, 2, // Top
		0, 3, 7, // Bot
		0, 7, 4, // Bot
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

	if (!InitializeTexture())
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(m_windowWidth);
	viewport.Height = static_cast<FLOAT>(m_windowHeight);
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
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

	D3D11_RASTERIZER_DESC rasterizerDescCullFront;
	ZeroMemory(&rasterizerDescCullFront, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDescCullFront.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDescCullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	hr = m_device->CreateRasterizerState(&rasterizerDescCullFront, m_rasterizerStateCullFront.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer Cull Front state.");

	return true;
}

bool Graphics::InitializeBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtbd;
	auto hr = m_device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

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
	COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");

	return true;
}

bool Graphics::InitializeTexture()
{
	/*
	* need CoInitialize to be called before working, spritefont might be the one calling according to internet.
	*/
	std::wstring pathToFile = L"Data\\Textures\\Grass Seamless Texture.jpg";
	auto hr = DirectX::CreateWICTextureFromFile(m_device.Get(), pathToFile.c_str(), nullptr, m_grassTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to grass Texture.");

	pathToFile = L"Data\\Textures\\blue.jpg";
	hr = DirectX::CreateWICTextureFromFile(m_device.Get(), pathToFile.c_str(), nullptr, m_blueTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to blue Texture.");

	pathToFile = L"Data\\Textures\\Bricks Seamless Texture.jpg";
	hr = DirectX::CreateWICTextureFromFile(m_device.Get(), pathToFile.c_str(), nullptr, m_pavementTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to Pavement Texture.");

	return true;
}

bool Graphics::InitializeConstantBuffers()
{
	auto resourceFactory = ResourceBufferFactory();

	auto simpleConstantBuffer = resourceFactory.CreateSimpleVsConstantBuffer(m_device);
	if (!simpleConstantBuffer)
	{
		return false;
	}

	auto simplePsConstantBuffer = resourceFactory.CreateSimplePsConstantBuffer(m_device);
	if (!simplePsConstantBuffer)
	{
		return false;
	}


	m_vsConstantBuffers.push_back(std::move(simpleConstantBuffer));
	m_psConstantBuffers.push_back(std::move(simplePsConstantBuffer));

	return true;
}

bool Graphics::UpdateDynamicVsConstantBuffer(const size_t index, CB_VS_vertexShader newData) const
{
	if (m_vsConstantBuffers.size() < index )
	{
		errorlogger::Log("setDynamicVsConstantBuffer, index higher an vector size");
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResourceVs;
	auto hr = m_deviceContext->Map(m_vsConstantBuffers.at(index)->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourceVs.pData, &newData, sizeof(CB_VS_vertexShader));
	m_deviceContext->Unmap(m_vsConstantBuffers.at(index)->GetBuffer(), index);
	m_deviceContext->VSSetConstantBuffers(0, 1, m_vsConstantBuffers.at(index)->GetBufferAddress());

	return false;
}

bool Graphics::UpdateDynamicPsConstantBuffer(const size_t index, CB_PS_pixelShader newData) const
{
	if (m_psConstantBuffers.size() < index)
	{
		errorlogger::Log("setDynamicPsConstantBuffer, index higher an vector size");
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResourcePs;
	auto hr = m_deviceContext->Map(m_psConstantBuffers.at(index)->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourcePs.pData, &newData, sizeof(CB_PS_pixelShader));
	m_deviceContext->Unmap(m_psConstantBuffers.at(index)->GetBuffer(), index);
	m_deviceContext->PSSetConstantBuffers(0, 1, m_psConstantBuffers.at(index)->GetBufferAddress());

	return false;
}

void Graphics::InitializeImGui(HWND hwnd) const
{
	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
	ImGui::StyleColorsDark();
}

void Graphics::DestroyImGui() const
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
