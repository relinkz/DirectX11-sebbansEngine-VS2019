#pragma once
#include "MouseEvent.h"

#include <queue>
#include <memory>
class Mouse
{
public:
	void OnLeftPressed(const MousePoint& mousePos);
	void OnLeftReleased(const MousePoint& mousePos);
	void OnRightPressed(const MousePoint& mousePos);
	void OnRightReleased(const MousePoint& mousePos);
	void OnMidPressed(const MousePoint& mousePos);
	void OnMidReleased(const MousePoint& mousePos);
	void OnWheelUp(const MousePoint& mousePos);
	void OnWheelDown(const MousePoint& mousePos);
	void OnMouseMove(const MousePoint& mousePos);
	void OnMouseMoveRaw(const MousePoint& mousePos);

	bool IsLeftDown() const;
	bool IsMidDown() const;
	bool IsRightDown() const;

	const MousePoint GetPos() const;
	bool EventBufferIsEmpty() const;
	const MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> m_eventBuffer;
	bool m_leftIsDown;
	bool m_rightIsDown;
	bool m_midbuttonDown;
	MousePoint m_pos;
};