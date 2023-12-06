#pragma once
#include "IResourceBuffer.h"
#include <memory>

class ResourceBufferFactory
{
public:
	std::unique_ptr<IResourceVertexBuffer> CreateSimpleVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<Vertex> vData);
	std::unique_ptr<IResourceIndexBuffer> CreateSimpleIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::vector<DWORD> iData);
	std::unique_ptr<IResourceConstantBuffer> CreateSimpleVsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	std::unique_ptr<IResourceConstantBuffer> CreateSimplePsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	std::unique_ptr<IResourceConstantBuffer> CreateMaterialPsConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device);
};