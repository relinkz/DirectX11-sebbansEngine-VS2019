#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() : m_pos(0.0f, 0.0f, 1.0f), m_color(1.0f, 1.0f, 1.0f) {}
	Vertex(const float x, const float y) : m_pos(x, y, 1.0f), m_color(1.0f, 1.0f, 1.0f) {}
	Vertex(const float x, const float y, const float r, const float g, const float b) : m_pos(x, y, 1.0f), m_color(r, g, b) {}
	Vertex(const float x, const float y, const float z, const float r, const float g, const float b) : m_pos(x, y, z), m_color(r, g, b){}

	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_color;
};