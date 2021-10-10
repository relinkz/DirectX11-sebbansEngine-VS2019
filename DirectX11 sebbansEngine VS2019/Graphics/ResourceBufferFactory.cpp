#include "ResourceBufferFactory.h"
#include "ResourceBuffer.h"

std::unique_ptr<IResourceBuffer> ResourceBufferFactory::CreatePointVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	std::unique_ptr<Simple3pVertexBuffer> vertexBuffer;
	vertexBuffer = std::make_unique<Simple3pVertexBuffer>();

	if (!vertexBuffer->Initialize(device))
	{
		return nullptr;
	}

	return std::move(vertexBuffer);
}
