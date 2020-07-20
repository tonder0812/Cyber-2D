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

	#ifdef CB_DEBUG
	struct Event {
		EventType Type;
		virtual std::string getName() const { return "Event"; }
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.getName();
	}
	#else
	struct Event {
		EventType Type;
	};
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os;
	}
	#endif
}

