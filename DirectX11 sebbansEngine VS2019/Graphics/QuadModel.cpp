#include "QuadModel.h"

using namespace DirectX;
using namespace std;

void QuadModel::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void QuadModel::SetPosition(const XMFLOAT3& pos)
{
	m_pos = pos;
}

void QuadModel::SetRotation(const XMFLOAT3& rot)
{
	m_rot = rot;
}

void QuadModel::SetScale(const XMFLOAT3& scale)
{
	m_scale = scale;
}

XMMATRIX QuadModel::GetWorldMatrix() const
{
	return XMMATRIX();
}

unique_ptr<IResourceVertexBuffer> QuadModel::GetResourceVertexBuffer()
{
	return unique_ptr<IResourceVertexBuffer>();
}
