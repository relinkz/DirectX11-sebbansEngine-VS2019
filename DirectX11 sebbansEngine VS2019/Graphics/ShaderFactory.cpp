#include "ShaderFactory.h"
#include "Shaders.h"

std::unique_ptr<IVertexShader> ShaderFactory::CreateDefaultVertexShader(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& filename)
{
	auto vertexShader = std::make_unique<VertexShader>();
	const std::wstring vertexShaderPath = vertexShader->GetShaderPath() + filename + L".cso";

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (!vertexShader->Initialize(device, vertexShaderPath, layout, ARRAYSIZE(layout)))
	{
		return nullptr;
	}

	return std::move(vertexShader);
}

std::unique_ptr<IPixelShader> ShaderFactory::CreateDefaultPixelShader(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& filename)
{
	auto pixelShader = std::make_unique<PixelShader>();
	const std::wstring pixelShaderPath = pixelShader->GetShaderPath() + filename + L".cso";
	if (!pixelShader->Initialize(device, pixelShaderPath))
	{
		return nullptr;
	}

	return std::move(pixelShader);
}