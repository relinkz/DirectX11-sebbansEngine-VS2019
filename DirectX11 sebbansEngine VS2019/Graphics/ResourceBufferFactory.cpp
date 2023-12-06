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

std::unique_ptr<IResourceConstantBuffer> ResourceBufferFactory::CreateSimpleVsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	std::unique_ptr<SimpleResourceVsConstantBuffer> constantBuffer;
	constantBuffer = std::make_unique<SimpleResourceVsConstantBuffer>();

	if (!constantBuffer->Initialize(device))
	{
		return nullptr;
	}

	return std::move(constantBuffer);
}

std::unique_ptr<IResourceConstantBuffer> ResourceBufferFactory::CreateSimplePsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	std::unique_ptr<SimpleResourcePsConstantBuffer> constantBuffer;
	constantBuffer = std::make_unique<SimpleResourcePsConstantBuffer>();

	if (!constantBuffer->Initialize(device))
	{
		return nullptr;
	}

	return std::move(constantBuffer);
}

std::unique_ptr<IResourceConstantBuffer> ResourceBufferFactory::CreateMaterialPsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	std::unique_ptr<MaterialPsConstantBuffer> constantBuffer;
	constantBuffer = std::make_unique<MaterialPsConstantBuffer>();

	if (!constantBuffer->Initialize(device))
	{
		return nullptr;
	}

	return std::move(constantBuffer);
}
