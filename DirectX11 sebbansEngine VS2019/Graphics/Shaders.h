#pragma once

#include "../ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	std::wstring GetShaderPath() const;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
};
