#include "ResourceBuffer.h"
#include "../ErrorLogger.h"

bool SimpleTriangleVertexBuffer::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData)
{
	m_nrOfVerticies = static_cast<UINT>(vData.size());

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; // Identify how the buffer is expected to be read from and written to. Frequency of update is a key factor
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_nrOfVerticies); // Size of the buffer in bytes.
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Identify how the buffer will be bound to the pipeline.
	vertexBufferDesc.CPUAccessFlags = 0; // CPU access flags (see D3D11_CPU_ACCESS_FLAG) or 0 if no CPU access is necessary
	vertexBufferDesc.MiscFlags = 0; // Miscellaneous flags (see D3D11_RESOURCE_MISC_FLAG) or 0 if unused

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vData.data();

	auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	return true;
}

UINT SimpleTriangleVertexBuffer::GetNrOfVerticies() const
{
	return m_nrOfVerticies;
}

ID3D11Buffer** SimpleTriangleVertexBuffer::GetBufferAddress()
{
	return m_vertexBuffer.GetAddressOf();
}

bool SimpleResourceIndexBuffer::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData)
{
	m_nrOfIndencies = static_cast<UINT>(iData.size());

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT; // Identify how the buffer is expected to be read from and written to. Frequency of update is a key factor
	indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(DWORD) * m_nrOfIndencies); // Size of the buffer in bytes.
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Identify how the buffer will be bound to the pipeline.
	indexBufferDesc.CPUAccessFlags = 0; // CPU access flags (see D3D11_CPU_ACCESS_FLAG) or 0 if no CPU access is necessary
	indexBufferDesc.MiscFlags = 0; // Miscellaneous flags (see D3D11_RESOURCE_MISC_FLAG) or 0 if unused

	D3D11_SUBRESOURCE_DATA indexBufferData;
	ZeroMemory(&indexBufferData, sizeof(indexBufferData));
	indexBufferData.pSysMem = iData.data();

	auto hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		errorlogger::Log(hr, "Failed to create index buffer.");
		return false;
	}

	return true;
}

UINT SimpleResourceIndexBuffer::GetNrOfIndencies() const
{
	return m_nrOfIndencies;
}

ID3D11Buffer** SimpleResourceIndexBuffer::GetBufferAddress()
{
	return m_indexBuffer.GetAddressOf();
}

ID3D11Buffer* SimpleResourceIndexBuffer::GetBuffer()
{
	return m_indexBuffer.Get();
}
