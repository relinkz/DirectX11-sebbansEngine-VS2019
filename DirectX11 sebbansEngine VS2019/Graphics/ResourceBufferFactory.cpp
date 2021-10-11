#include "ResourceBufferFactory.h"
#include "ResourceBuffer.h"

std::unique_ptr<IResourceVertexBuffer> ResourceBufferFactory::CreateSimpleVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData)
{
	std::unique_ptr<SimpleResourceVertexBuffer> vertexBuffer;
	vertexBuffer = std::make_unique<SimpleResourceVertexBuffer>();

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

std::unique_ptr<IResourceConstantBuffer> ResourceBufferFactory::CreateSimpleConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	std::unique_ptr<SimpleResourceConstantBuffer> constantBuffer;
	constantBuffer = std::make_unique<SimpleResourceConstantBuffer>();

	if (!constantBuffer->Initialize(device))
	{
		return nullptr;
	}

	return std::move(constantBuffer);
}
