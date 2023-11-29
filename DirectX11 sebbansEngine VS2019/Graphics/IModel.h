#pragma once
#include "IResourceBuffer.h"
#include <DirectXMath.h>
#include <memory>
class IModel
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device) = 0;
	virtual void SetPosition(const DirectX::XMFLOAT3&) = 0;
	virtual void SetRotation(const DirectX::XMFLOAT3&) = 0;
	virtual void SetScale(const DirectX::XMFLOAT3&) = 0;

	virtual DirectX::XMMATRIX GetWorldMatrix() const = 0;
	virtual std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer() = 0;
};