#pragma once
#include "IShaders.h"
#include <memory>

class ShaderFactory
{
public:
	std::unique_ptr<IVertexShader> CreateDefaultVertexShader(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& filename);
	std::unique_ptr<IPixelShader> CreateDefaultPixelShader(Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& filename);
};
