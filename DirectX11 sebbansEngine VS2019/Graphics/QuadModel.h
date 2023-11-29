#pragma once
#include "IModel.h"

class QuadModel : public IModel
{
private:
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_pos;

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>&) override;
	void SetPosition(const DirectX::XMFLOAT3&) override;
	void SetRotation(const DirectX::XMFLOAT3&) override;
	void SetScale(const DirectX::XMFLOAT3&) override;

	DirectX::XMMATRIX GetWorldMatrix() const override;
	std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer() override;
};