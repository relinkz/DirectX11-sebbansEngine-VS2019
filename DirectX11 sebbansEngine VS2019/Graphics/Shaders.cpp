#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT nrElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load Vertex Shader File.");

	hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to Create Vertex Shader.");


	hr = device->CreateInputLayout(layoutDesc, nrElements,
		GetBuffer()->GetBufferPointer(),
		GetBuffer()->GetBufferSize(),
		m_inputLayout.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create Input layout.");

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return m_shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return m_shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return m_inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load Pixel Shader File.");

	hr = device->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to Create Pixel Shader.");

	return true;
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return m_shaderBuffer.Get();
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return m_shader.Get();
}
