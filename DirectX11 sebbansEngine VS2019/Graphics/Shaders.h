#pragma once

#include "../ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT nrElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
	std::wstring GetShaderPath() const;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
