#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;

	const DirectX::XMVECTOR& GetPositionVector() const;
	const DirectX::XMFLOAT3& GetPositionFloat3() const;
	const DirectX::XMVECTOR& GetRotationVector() const;
	const DirectX::XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(const float x, const float y, const float z);
	void AdjustPosition(const DirectX::XMVECTOR& pos);
	void AdjustPosition(const float x, const float y, const float z);
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(const float x, const float y, const float z);
	void AdjustRotation(const DirectX::XMVECTOR& pos);
	void AdjustRotation(const float x, const float y, const float z);
	void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);

private:
	void UpdateViewMatrix();
	DirectX::XMVECTOR m_posVector;
	DirectX::XMVECTOR m_rotVector;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_rot;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};

