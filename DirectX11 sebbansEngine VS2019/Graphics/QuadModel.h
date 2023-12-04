#pragma once
#include "IModel.h"

class QuadModel : public IModel
{
private:
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMMATRIX m_tMtx;
	std::vector<Vertex> m_vertecies;
public:
	void Initialize() override;
	void SetPosition(const DirectX::XMFLOAT3&) override;
	void SetRotation(const DirectX::XMFLOAT3&) override;
	void SetScale(const DirectX::XMFLOAT3&) override;

	DirectX::XMMATRIX GetWorldMatrix() const override;
	std::unique_ptr<IResourceVertexBuffer> GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>&) override;
};