#pragma once
#include <DirectXMath.h>

namespace GraphicsTools
{
	static const DirectX::XMFLOAT3 SubtractFloat3(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
	{
		DirectX::XMFLOAT3 result = DirectX::XMFLOAT3(a);

		result.x -= b.x;
		result.y -= b.y;
		result.z -= b.z;

		return result;
	}

	static const DirectX::XMFLOAT3 AddFloat3(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
	{
		DirectX::XMFLOAT3 result = DirectX::XMFLOAT3(a);

		result.x += b.x;
		result.y += b.y;
		result.z += b.z;

		return result;
	}
}
