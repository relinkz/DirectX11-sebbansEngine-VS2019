#pragma once
#include "IResourceBuffer.h"
#include <DirectXMath.h>
#include <memory>
#include <string>

class IModel
{
public:
	virtual void Initialize() = 0;
	virtual void SetPosition(const DirectX::XMFLOAT3&) = 0;
	virtual void SetRotation(const DirectX::XMFLOAT3&) = 0;
	virtual void SetScale(const DirectX::XMFLOAT3&) = 0;

	virtual DirectX::XMMATRIX GetWorldMatrix() const = 0;
	virtual std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device) = 0;
	virtual std::vector<std::wstring> GetDiffuseMaps() const = 0;
};