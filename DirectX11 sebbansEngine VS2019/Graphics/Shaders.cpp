#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr < ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT nrElements)
{
  HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), m_shaderBuffer.GetAddressOf());
  if (FAILED(hr))
  {
    std::wstring errorMsg = L"Failed to load Vertex Shader File: ";
    errorMsg += shaderPath;
    errorlogger::Log(hr, errorMsg);
    return false;
  }

  hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
  if (FAILED(hr))
  {
    std::wstring errorMsg = L"Failed to Create Vertex Shader: ";
    errorlogger::Log(hr, errorMsg);
    return false;
  }

  hr = device->CreateInputLayout(layoutDesc, nrElements,
    GetBuffer()->GetBufferPointer(),
    GetBuffer()->GetBufferSize(),
    m_inputLayout.GetAddressOf());

  if (FAILED(hr))
  {
    errorlogger::Log(hr, "Failed to create Input layout");
    return false;
  }

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

std::wstring VertexShader::GetShaderPath() const
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
