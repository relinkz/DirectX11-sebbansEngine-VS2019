#include "Graphics.h"
#include "ShaderFactory.h"
#include "ResourceBufferFactory.h"
#include "AdapterReader.h"
#include "ModelFactory.h"

#include "SwapChain.h"

static float s_focusObjAlpha = 1.0f;

static float s_focusObjRot[3] = { 0.0f, 0.0f, 0.0f };
static float s_focusObjTrans[3] = { 0.0f, 0.0f, 0.0f };
static float s_focusObjScale[3] = { 1.0f, 1.0f, 1.0f };
static float s_lightPos[3] = {0.0f, 10.0f, 0.0f };

static bool s_contRotate = false;
static DirectX::XMFLOAT3 s_rotateVec = DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XM_PI/4.0f);

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
	gameCamera->SetPosition(0.0f, 0.0f, -4.0f);

	float rotationDegrees = 90.0f;
	float aspectRatio = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	gameCamera->SetProjectionValues(90.0f, aspectRatio, nearZ, farZ);

	m_fpsTimer = std::make_unique<Timer>();
	m_fpsTimer->Start();

	return true;
}

void Graphics::RenderFrame()
{
	PreparePipeline();

	StartRender();

	RenderImGui();

	m_swapChain->Present(0, NULL);
}

Graphics::~Graphics()
{
	DestroyImGui();
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	m_swapChain = swapChain::newSwapChain(hwnd,
		m_windowWidth,
		m_windowHeight,
		m_device, m_deviceContext);
	
	if (!m_swapChain){
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

bool Graphics::InitializeRenderTargetViewWithSwapchain()
{
	// get backbuffer as a 2D texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
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

bool Graphics::InitializeScene()
{
	auto modelFactory = ModelFactory();
	auto model = modelFactory.CreateBox(m_device, m_deviceContext);
	model->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	m_modelsInScene.emplace_back(move(model));

	//CreateGroundQuads();

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

bool Graphics::InitializeConstantBuffers()
{
	auto resourceFactory = ResourceBufferFactory();

	auto simpleConstantBuffer = resourceFactory.CreateSimpleVsConstantBuffer(m_device);
	if (!simpleConstantBuffer)
	{
		return false;
	}

	auto simpleCMatrixVsBuffer = resourceFactory.CreateSimpleVsConstantBuffer(m_device);
	if (!simpleCMatrixVsBuffer)
	{
		return false;
	}

	auto simplePsConstantBuffer = resourceFactory.CreateSimplePsConstantBuffer(m_device);
	if (!simplePsConstantBuffer)
	{
		return false;
	}

	auto materialPsConstantBuffer = resourceFactory.CreateMaterialPsConstantBuffer(m_device);
	if (!materialPsConstantBuffer)
	{
		return false;
	}

	m_vsConstantBuffers.push_back(std::move(simpleConstantBuffer));
	m_vsConstantBuffers.push_back(std::move(simpleCMatrixVsBuffer));
	m_psConstantBuffers.push_back(std::move(simplePsConstantBuffer));
	m_psConstantBuffers.push_back(std::move(materialPsConstantBuffer));

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
	m_deviceContext->VSSetConstantBuffers(index, 1, m_vsConstantBuffers.at(index)->GetBufferAddress());

	return false;
}

bool Graphics::UpdateDynamicPsConstantBuffer(const size_t index, CB_PS_pixelAlphaShader newData) const
{
	if (m_psConstantBuffers.size() < index)
	{
		errorlogger::Log("setDynamicPsConstantBuffer, index higher an vector size");
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResourcePs;
	auto hr = m_deviceContext->Map(m_psConstantBuffers.at(index)->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourcePs.pData, &newData, sizeof(CB_PS_pixelAlphaShader));
	m_deviceContext->Unmap(m_psConstantBuffers.at(index)->GetBuffer(), 0);
	m_deviceContext->PSSetConstantBuffers(0, 1, m_psConstantBuffers.at(index)->GetBufferAddress());

	return true;
}

bool Graphics::UpdateDynamicPsConstantBuffer(const size_t index, CB_PS_pixelMaterialShader newData) const
{
	if (m_psConstantBuffers.size() < index)
	{
		errorlogger::Log("setDynamicPsConstantBuffer, index higher an vector size");
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResourcePs;
	auto hr = m_deviceContext->Map(m_psConstantBuffers.at(index)->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourcePs.pData, &newData, sizeof(CB_PS_pixelMaterialShader));
	m_deviceContext->Unmap(m_psConstantBuffers.at(index)->GetBuffer(), index);
	m_deviceContext->PSSetConstantBuffers(1, 1, m_psConstantBuffers.at(index)->GetBufferAddress());

	return true;
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

void Graphics::UpdateCameraCB() const
{
	CB_VS_vertexShader cCameraMatrix;
	cCameraMatrix.m_matrix = gameCamera->GetWorldViewProjectionMatrix();
	cCameraMatrix.m_view = gameCamera->GetViewMatrix();

	UpdateDynamicVsConstantBuffer(0, cCameraMatrix);
}

void Graphics::CreateGroundQuads()
{
	auto modelFactory = ModelFactory();

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			auto ground = modelFactory.CreateQuadModel(m_device, m_deviceContext);
			ground->SetRotation(DirectX::XMFLOAT3(DirectX::XM_PIDIV2, 0.0f, 0.0f));
			ground->SetPosition(DirectX::XMFLOAT3(0.0f, -0.6f, j * 2.0f * 5.0f));
			ground->SetScale(DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f));

			auto groundVb = ground->GetResourceVertexBuffer(m_device);
			m_modelsInScene.emplace_back(move(ground));
		}
	}
}

void Graphics::PreparePipeline() const
{
	float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgColor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf()); // see pixel shader register

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	m_deviceContext->OMSetBlendState(m_blendState.Get(), NULL, 0xFFFFFFFF);
}

void Graphics::RenderImGui() const
{
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
	m_spriteFont->DrawString(m_spriteBatch.get(), helpers::strings::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	m_spriteBatch->End();

	// start the imgui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// create test window
	ImGui::Begin("Object transform");
	ImGui::DragFloat("Alpha:", &s_focusObjAlpha, 0.01f, 0, 1.0f);

	ImGui::DragFloat3("Scale", s_focusObjScale, 0.01f, 0.1f, 100.0f);
	ImGui::DragFloat3("Rotation", s_focusObjRot, 0.01f, -2.0f * DirectX::XM_PI, 2.0f * DirectX::XM_PI);
	ImGui::DragFloat3("Translation", s_focusObjTrans, 0.01, -100.0f, 100.0f);
	ImGui::DragFloat3("Light position", s_lightPos, 0.01, -100.0f, 100.0f);

	ImGui::Checkbox("Rotate x", &s_contRotate);

	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::StartRender()
{
	if (s_contRotate == false)
	{
		DirectX::XMFLOAT3 objScale = { s_focusObjScale[0], s_focusObjScale[1], s_focusObjScale[2] };
		DirectX::XMFLOAT3 objRot = { s_focusObjRot[0], s_focusObjRot[1], s_focusObjRot[2] };
		DirectX::XMFLOAT3 objTrans = { s_focusObjTrans[0], s_focusObjTrans[1], s_focusObjTrans[2] };


		m_modelsInScene.at(0)->SetScale(objScale);
		m_modelsInScene.at(0)->SetRotation(objRot);
		m_modelsInScene.at(0)->SetPosition(objTrans);
	}
	else
	{
		s_rotateVec.y += 0.001;
		if (s_rotateVec.y > DirectX::XM_2PI)
		{
			s_rotateVec.y = 0.0f;
		}
		m_modelsInScene.at(0)->SetRotation(s_rotateVec);
	}

	UpdateCameraCB();

	CB_PS_pixelAlphaShader cPsData;
	cPsData.alpha = DirectX::XMFLOAT4(s_focusObjAlpha, 0.0f, 0.0f, 0.0f);
	cPsData.lightPos = DirectX::XMFLOAT4(s_lightPos[0], s_lightPos[1], s_lightPos[2], 0.0f);
	cPsData.lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	cPsData.cameraPos = gameCamera->GetPositionFloat4();
	UpdateDynamicPsConstantBuffer(0, cPsData);

	// i need "object world matrix cb in vertex shader"
	// to render an object.
	// i need "Local verticies"
	// i need "I need objTexture in ps shader"

	for (size_t i = 0; i < m_modelsInScene.size() ; i++)
	{		
		m_deviceContext->RSSetState(m_rasterizerStateCullFront.Get());
		m_deviceContext->RSSetState(m_rasterizerState.Get());

		m_modelsInScene.at(i)->Draw(m_deviceContext);
	}
}