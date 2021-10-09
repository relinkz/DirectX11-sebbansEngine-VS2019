#include "Mouse.h"

void Mouse::OnLeftPressed(const MousePoint& mousePos)
{
	m_leftIsDown = true;
	m_eventBuffer.push({ MouseEvent::EventType::LPress, mousePos });
}

void Mouse::OnLeftReleased(const MousePoint& mousePos)
{
	m_leftIsDown = false;
	m_eventBuffer.push({ MouseEvent::EventType::LRelease, mousePos });
}

void Mouse::OnRightPressed(const MousePoint& mousePos)
{
	m_rightIsDown = true;
	m_eventBuffer.push({ MouseEvent::EventType::RPress, mousePos });
}

void Mouse::OnRightReleased(const MousePoint& mousePos)
{
	m_rightIsDown = false;
	m_eventBuffer.push({ MouseEvent::EventType::RRelease, mousePos });
}

void Mouse::OnMidPressed(const MousePoint& mousePos)
{
	m_midbuttonDown = true;
	m_eventBuffer.push({ MouseEvent::EventType::MPress, mousePos });
}

void Mouse::OnMidReleased(const MousePoint& mousePos)
{
	m_midbuttonDown = false;
	m_eventBuffer.push({ MouseEvent::EventType::MRelease, mousePos });
}

void Mouse::OnWheelUp(const MousePoint& mousePos)
{
	m_eventBuffer.push({ MouseEvent::EventType::WheelUp, mousePos });
}

void Mouse::OnWheelDown(const MousePoint& mousePos)
{
	m_eventBuffer.push({ MouseEvent::EventType::WheelDown, mousePos });
}

void Mouse::OnMouseMove(const MousePoint& mousePos)
{
	m_pos = mousePos;
	m_eventBuffer.push({ MouseEvent::EventType::Move, mousePos });
}

void Mouse::OnMouseMoveRaw(const MousePoint& mousePos)
{
	m_eventBuffer.push({ MouseEvent::EventType::RAW_MOVE, mousePos });
}

bool Mouse::IsLeftDown() const
{
	return m_leftIsDown;
}

bool Mouse::IsMidDown() const
{
	return m_midbuttonDown;
}

bool Mouse::IsRightDown() const
{
	return m_rightIsDown;
}

const MousePoint Mouse::GetPos() const
{
	return this->m_pos;
}

bool Mouse::EventBufferIsEmpty() const
{
	return m_eventBuffer.empty();
}

const MouseEvent Mouse::ReadEvent()
{
	if (m_eventBuffer.empty())
	{
		return MouseEvent();
	}

	MouseEvent mouseEvent = m_eventBuffer.front();
	m_eventBuffer.pop();
	return std::move(mouseEvent);
}
