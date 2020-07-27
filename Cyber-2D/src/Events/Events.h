#pragma once
#include "pch.h"

namespace Cyber {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	struct Event {
		EventType Type=EventType::None;
		Event():Type(EventType::None){}
		virtual ~Event() = default;
		virtual std::string getName() const { return "Event"; }
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.getName();
	}
}

