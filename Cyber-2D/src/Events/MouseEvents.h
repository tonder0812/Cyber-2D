#pragma once
#include "Events.h"
#include "Core\Keycodes.h"

namespace Cyber {
	struct MouseMovedEvent : public Event
	{
		double x, y;
		MouseMovedEvent(double x_, double y_) :
			x(x_), y(y_) {
			Type = EventType::MouseMoved;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Mouse Moved Event X: " << x << " Y: " << y;
			return s.str();
		}
		#endif
	};

	struct MousePressedEvent : public Event
	{
		int button;
		MousePressedEvent(int button_) :
			button(button_) {
			Type = EventType::MouseButtonPressed;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Mouse Pressed Event button: " << button;
			return s.str();
		}
		#endif
	};
	struct MouseReleasedEvent : public Event
	{
		int button;
		MouseReleasedEvent(int button_) :
			button(button_) {
			Type = EventType::MouseButtonReleased;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Mouse Released Event button: " << button;
			return s.str();
		}
		#endif
	};

	struct MouseScrolledEvent : public Event
	{
		double xoff, yoff;
		MouseScrolledEvent(double xoff_, double yoff_) :
			xoff(xoff_), yoff(yoff_) {
			Type = EventType::MouseScrolled;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Mouse Scrolled Event X offset: " << xoff << " Y offset: " << yoff;
			return s.str();
		}
		#endif
	};
}
