#pragma once
#include "IResourceBuffer.h"
#include <memory>

class SimpleResourceVertexBuffer : public IResourceVertexBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData) override;
	UINT GetNrOfVerticies() const override;
	ID3D11Buffer** GetBufferAddress() override;
	const UINT GetStride() const override;

private:
	// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_nrOfVerticies = 0;
	UINT m_stride;
};

class SimpleResourceIndexBuffer : public IResourceIndexBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData) override;
	UINT GetNrOfIndencies() const override;
	ID3D11Buffer** GetBufferAddress() override;
	ID3D11Buffer* GetBuffer() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_nrOfIndencies = 0;
};

class SimpleResourceVsConstantBuffer : public IResourceConstantBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device) override;
	ID3D11Buffer** GetBufferAddress() override;
	ID3D11Buffer* GetBuffer() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};

class SimpleResourcePsConstantBuffer : public IResourceConstantBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device) override;
	ID3D11Buffer** GetBufferAddress() override;
	ID3D11Buffer* GetBuffer() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};