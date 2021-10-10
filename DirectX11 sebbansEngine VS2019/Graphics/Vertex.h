#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : m_pos(0, 0), m_color(1.0f, 1.0f, 1.0f) {}
	Vertex(const float x, const float y) : m_pos(x, y), m_color(1.0f, 1.0f, 1.0f) {}
	Vertex(const float x, const float y, const float r, const float g, const float b) : m_pos(x, y), m_color(r, g, b){}

	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT3 m_color;
};