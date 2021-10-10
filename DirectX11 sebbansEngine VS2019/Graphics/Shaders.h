#pragma once

#include "../ErrorLogger.h"
#include "IShaders.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>

class VertexShader : public IVertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT nrElements) override;
	ID3D11VertexShader* GetShader() override;
	ID3D10Blob* GetBuffer() override;
	ID3D11InputLayout* GetInputLayout() override;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

class PixelShader : public IPixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath) override;
	ID3D10Blob* GetBuffer() override;
	ID3D11PixelShader* GetShader() override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
};
