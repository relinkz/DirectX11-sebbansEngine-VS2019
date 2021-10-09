#pragma once
#include "KeyboardEvent.h"
#include <queue>

class Keyboard
{
public:
	Keyboard();

	bool KeyIsPressed(const unsigned char keycode) const;
	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;

	KeyboardEvent ReadKey();
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatKeys();
	void DisableAutoRepeatChars();
	bool IsKeysAutoRepeat() const;
	bool IsCharsAutoRepeat() const;

private:
	bool m_autoRepeatKeys;
	bool m_autoRepeatChars;
	bool m_keyStates[256];
	std::queue<KeyboardEvent> m_keyBuffer;
	std::queue<unsigned char> m_charBuffer;
};

