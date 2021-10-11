#include "ResourceBufferFactory.h"
#include "ResourceBuffer.h"

std::unique_ptr<IResourceBuffer> ResourceBufferFactory::CreateSimpleTriangleVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData)
{
	std::unique_ptr<SimpleTriangleVertexBuffer> vertexBuffer;
	vertexBuffer = std::make_unique<SimpleTriangleVertexBuffer>();

	if (!vertexBuffer->Initialize(device, vData))
	{
		return nullptr;
	}

	return std::move(vertexBuffer);
}

std::unique_ptr<IResourceIndexBuffer> ResourceBufferFactory::CreateSimpleIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData)
{
	std::unique_ptr<SimpleResourceIndexBuffer> indexBuffer;
	indexBuffer = std::make_unique<SimpleResourceIndexBuffer>();

	if (!indexBuffer->Initialize(device, iData))
	{
		return nullptr;
	}

	return std::move(indexBuffer);
}
