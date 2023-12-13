#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMFLOAT4& GetPositionFloat4() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;
	const XMMATRIX& GetWorldMatrix() const;
	const XMMATRIX& GetWorldViewProjectionMatrix() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const float x, const float y, const float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const float x, const float y, const float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const float x, const float y, const float z);
	void AdjustRotation(const XMVECTOR& pos);
	void AdjustRotation(const float x, const float y, const float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);

	const XMVECTOR& GetForwardVector() const;
	const XMVECTOR& GetBackwardVector() const;
	const XMVECTOR& GetRightVector() const;
	const XMVECTOR& GetLeftVector() const;


private:
	void UpdateViewMatrix();
	XMVECTOR m_posVector;
	XMVECTOR m_rotVector;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_rot;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR m_vec_forward;
	XMVECTOR m_vec_left;
	XMVECTOR m_vec_right;
	XMVECTOR m_vec_backward;
};

