#include "ResourceBuffer.h"
#include "../ErrorLogger.h"

bool SimpleTriangleVertexBuffer::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData)
{
	m_nrOfVerticies = vData.size();

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
		errorlogger::Log(hr, "Failed to create buffer.");
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