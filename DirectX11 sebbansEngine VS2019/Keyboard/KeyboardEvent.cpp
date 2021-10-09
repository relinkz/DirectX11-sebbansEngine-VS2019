#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
	: m_type(EventType::Invalid)
	, m_key('0')
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
	: m_type(type)
	, m_key(key)
{
}

bool KeyboardEvent::IsPress() const
{
	return m_type == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
	return m_type == EventType::Release;
}

bool KeyboardEvent::isValid() const
{
	return m_type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return m_key;
}
