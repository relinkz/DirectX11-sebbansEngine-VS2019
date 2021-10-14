#include "Camera.h"
#include "GraphicsTools.h"

using namespace DirectX;

Camera::Camera()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_posVector = XMLoadFloat3(&m_pos);
	m_rotVector = XMLoadFloat3(&m_rot);

	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	const float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return m_posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return m_pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return m_rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return m_rot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&m_pos, pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(const float x, const float y, const float z)
{
	m_pos = XMFLOAT3(x, y, z);
	m_posVector = XMLoadFloat3(&m_pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	m_posVector += pos;
	XMStoreFloat3(&m_pos, m_posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const float x, const float y, const float z)
{
	m_pos.x += x;
	m_pos.y += y;
	m_pos.z += z;

	m_posVector = XMLoadFloat3(&m_pos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	m_rotVector = rot;
	XMStoreFloat3(&m_rot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(const float x, const float y, const float z)
{
	m_rot = XMFLOAT3(x, y, z);
	m_rotVector = XMLoadFloat3(&m_rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	m_rotVector += rot;
	XMStoreFloat3(&m_rot, m_rotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const float x, const float y, const float z)
{
	m_rot.x += x;
	m_rot.y += y;
	m_rot.z += z;

	m_rotVector = XMLoadFloat3(&m_rot);
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
	bool sameAsCamPos = lookAtPos.x == m_pos.x && lookAtPos.y == m_pos.y && lookAtPos.z == m_pos.z;
	if (sameAsCamPos)
	{
		return;
	}

	XMFLOAT3 dirVector = GraphicsTools::SubtractFloat3(m_pos, lookAtPos);
	
	float pitch = 0.0f;
	if (dirVector.y != 0.0f)
	{
		double dirX = dirVector.x;
		double dirZ = dirVector.z;
		
		dirX *= dirX;
		dirZ *= dirZ;

		const float distance = sqrt(dirX + dirZ);
		pitch = atan(dirVector.y / distance);
	}

	float yaw = 0.0f;
	if (dirVector.x != 0)
	{
		yaw = atan(dirVector.x / dirVector.z);
	}
	if (dirVector.z > 0 )
	{
		yaw += XM_PI;
	}

	this->SetRotation(pitch, yaw, 0.0f);
}

void Camera::UpdateViewMatrix()
{
	// Calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYawFromVector(m_rotVector);
	
	// Calculate unit vector of cam target based off camera forward value transformed by cam rotation
	// We are CHANGING the lookAt target depending on the rotation of the camera
	XMVECTOR lookAtTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	
	// Rotation first! then we need to adjust the lookaAt vector by translating aswell, scale->rot->translation.
	// In short: Adjust cam target to be offset by the camera's current position
	lookAtTarget += m_posVector;

	// Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	// Rebuild
	m_viewMatrix = XMMatrixLookAtLH(m_posVector, lookAtTarget, upDir);
}
