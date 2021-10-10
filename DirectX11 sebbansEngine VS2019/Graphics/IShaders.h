#pragma once
#include <string>
#include <wrl/client.h>
#include <d3d11.h>

class IShader
{
public:
	virtual ID3D10Blob* GetBuffer() = 0;
	static std::wstring GetShaderPath()
	{
		std::wstring ShaderPath = L"";
#pragma region DetermineShaderPath
		if (IsDebuggerPresent())
		{
#ifdef _DEBUG
#ifdef _WIN64
			ShaderPath = L"..\\x64\\Debug\\";
#else
			ShaderPath = L"..\\Debug\\";
#endif // WIN64
#else // release
#ifdef _WIN64
			ShaderPath = L"..\\x64\\Release\\";
#else
			ShaderPath = L"..\\Release\\";
#endif
#endif // DEBUG
		}
		return ShaderPath;
	}
};

class IVertexShader : public IShader
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT nrElements) = 0;
	virtual ID3D11VertexShader* GetShader() = 0;
	virtual ID3D11InputLayout* GetInputLayout() = 0;
};

class IPixelShader : public IShader
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath) = 0;
	virtual ID3D11PixelShader* GetShader() = 0;
};