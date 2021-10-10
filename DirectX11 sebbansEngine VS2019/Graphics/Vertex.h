#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : m_pos(0, 0) {}
	Vertex(float x, float y) : m_pos(x, y) {}

	DirectX::XMFLOAT2 m_pos;
};