#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : m_pos(0.0f, 0.0f, 1.0f), m_color(1.0f, 1.0f, 1.0f), m_texCoord(0.0f, 0.0f) {}

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_color;
	DirectX::XMFLOAT2 m_texCoord;
};

struct CB_VS_vertexShader
{
	DirectX::XMMATRIX m_matrix;
};

struct CB_PS_pixelAlphaShader
{
	float alpha = 1.0f;
};

struct CB_PS_pixelMaterialShader
{
	DirectX::XMFLOAT4 Ka;
	DirectX::XMFLOAT4 Kd;
	DirectX::XMFLOAT4 Ks;
	DirectX::XMFLOAT4 Ns;
};