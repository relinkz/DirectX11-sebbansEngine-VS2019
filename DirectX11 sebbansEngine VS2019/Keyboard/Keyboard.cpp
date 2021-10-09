#include "Keyboard.h"

Keyboard::Keyboard()
{
    for (int i = 0; i < 256; i++)
    {
        this->m_keyStates[i] = false;
    }
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) const
{
    return m_keyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty() const
{
    return m_keyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty() const
{
    return m_charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
    if (m_keyBuffer.empty())
    {
        return KeyboardEvent(); // empty event
    }
    KeyboardEvent event = m_keyBuffer.front();
    m_keyBuffer.pop();
    return event;
}

unsigned char Keyboard::ReadChar()
{
    if (m_charBuffer.empty())
    {
        return static_cast<unsigned char>(0);
    }
    unsigned char event = m_charBuffer.front();
    m_charBuffer.pop();
    return event;
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
    m_keyStates[key] = true;
    m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
    m_keyStates[key] = false;
    m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
    m_charBuffer.push(key);
}

void Keyboard::EnableAutoRepeatKeys()
{
    m_autoRepeatKeys = true;
}

void Keyboard::EnableAutoRepeatChars()
{
    m_autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
    m_autoRepeatKeys = false;
}

void Keyboard::DisableAutoRepeatChars()
{
    m_autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
    return m_autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
    return m_autoRepeatChars;
}
