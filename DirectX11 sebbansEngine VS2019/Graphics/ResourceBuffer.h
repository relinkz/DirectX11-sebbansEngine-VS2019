#pragma once
#include "IResourceBuffer.h"

class SimpleResourceVertexBuffer : public IResourceVertexBuffer
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData) override;
	virtual UINT GetNrOfVerticies() const override;
	virtual ID3D11Buffer** GetBufferAddress() override;
private:
	// A buffer interface accesses a buffer resource, which is unstructured memory. Buffers typically store vertex or index data.
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_nrOfVerticies = 0;
};

class SimpleResourceIndexBuffer : public IResourceIndexBuffer
{
public:
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData) override;
	virtual UINT GetNrOfIndencies() const override;
	virtual ID3D11Buffer** GetBufferAddress() override;
	virtual ID3D11Buffer* GetBuffer() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_nrOfIndencies = 0;
};