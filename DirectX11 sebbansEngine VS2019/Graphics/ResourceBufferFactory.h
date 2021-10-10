#pragma once
#include "IResourceBuffer.h"
#include "Vertex.h"
#include <memory>

class ResourceBufferFactory
{
public:
	std::unique_ptr<IResourceBuffer> CreateSimpleTriangleVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData);
};