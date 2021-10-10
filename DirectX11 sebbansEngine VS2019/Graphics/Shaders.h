#pragma once

#include "../ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class IShader
{
public:
	virtual ID3D10Blob* GetBuffer() = 0;
	virtual std::wstring GetShaderPath() const;
};

class VertexShader : public IShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT nrElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer() override;
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

class PixelShader : public IShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath);
	ID3D10Blob* GetBuffer() override;
	ID3D11PixelShader* GetShader();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
};
