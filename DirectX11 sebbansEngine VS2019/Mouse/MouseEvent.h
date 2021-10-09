#pragma once
struct MousePoint
{
	unsigned int x;
	unsigned int y;
};

class MouseEvent
{
public:
	enum class EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};

private:
	EventType m_type;
	MousePoint m_mousePoint;
public:
	MouseEvent();
	MouseEvent(const EventType type, const MousePoint mousePos);
	bool IsValid() const;
	const EventType GetType() const;
	const MousePoint GetPos() const;
};
