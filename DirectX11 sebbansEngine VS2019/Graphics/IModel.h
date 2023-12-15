#pragma once
#include "IResourceBuffer.h"
#include "IShaders.h"

#include <DirectXMath.h>
#include <memory>
#include <string>

class IModel
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dCtx) = 0;
	virtual void SetPosition(const DirectX::XMFLOAT3&) = 0;
	virtual void SetRotation(const DirectX::XMFLOAT3&) = 0;
	virtual void SetScale(const DirectX::XMFLOAT3&) = 0;

	virtual DirectX::XMMATRIX GetWorldMatrix() const = 0;
	virtual DirectX::XMMATRIX GetRotationMatrix() const = 0;

	virtual std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device) = 0;

	virtual void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx) const = 0;
	// material stuff
	virtual std::vector<std::wstring> GetDiffuseMaps() const = 0;
	virtual std::vector<std::wstring> GetNormalMaps() const = 0;
	virtual std::vector<std::wstring> GetOcclusionMaps() const = 0;
	virtual std::vector<std::wstring> GetSpecularMaps() const = 0;

	virtual DirectX::XMFLOAT4 GetKa() const = 0;
	virtual DirectX::XMFLOAT4 GetKd() const = 0;
	virtual DirectX::XMFLOAT4 GetKs() const = 0;
	virtual DirectX::XMFLOAT4 GetNs() const = 0;
};