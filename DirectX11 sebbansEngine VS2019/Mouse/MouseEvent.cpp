#include "MouseEvent.h"

MouseEvent::MouseEvent() :
	m_type(EventType::Invalid),
	m_mousePoint{ 0,0 }
{
}

MouseEvent::MouseEvent(const EventType type, const MousePoint mousePos) :
	m_type(type),
	m_mousePoint(mousePos)
{
}

bool MouseEvent::IsValid() const
{
	return m_type != EventType::Invalid;
}

const MouseEvent::EventType MouseEvent::GetType() const
{
	return m_type;
}

const MousePoint MouseEvent::GetPos() const
{
	return m_mousePoint;
}
