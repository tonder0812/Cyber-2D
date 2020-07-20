#pragma once
#include "Events.h"

namespace Cyber {
	struct WindowCloseEvent : public Event
	{
		WindowCloseEvent() {
			Type = EventType::WindowClose;
		};
		#ifdef CB_DEBUG
		std::string getName() const override {
			return "Window Close Event";
		}
		#endif
	};

	struct WindowResizeEvent : public Event
	{
		unsigned int width = 0, height = 0;
		WindowResizeEvent(unsigned int width_ = 0, unsigned int height_ = 0) :
			width(width_), height(height_) {
			Type = EventType::WindowResize;
		};
		#ifdef CB_DEBUG
		std::string getName() const override {
			std::stringstream s;
			s << "Window Resize Event Width:" << width << " Height:" << height;
			return s.str();
		}
		#endif
	};

	struct WindowFocusEvent : public Event
	{
		WindowFocusEvent() {
			Type = EventType::WindowFocus;
		};
		#ifdef CB_DEBUG
		std::string getName() const override {
			return "Window Focus Event";
		}
		#endif
	};

	struct WindowLostFocusEvent : public Event
	{
		WindowLostFocusEvent() {
			Type = EventType::WindowLostFocus;
		};
		#ifdef CB_DEBUG
		std::string getName() const override {
			return "Window Lost Focus Event";
		}
		#endif
	};

	struct WindowMovedEvent : public Event
	{
		unsigned int x = 0, y = 0;
		WindowMovedEvent(unsigned int x_ = 0, unsigned int y_ = 0) :
			x(x_), y(y_) {
			Type = EventType::WindowMoved;
		};
		#ifdef CB_DEBUG
		std::string getName() const override {
			std::stringstream s;
			s << "Window Moved Event X:" << x << " Y:" << y;
			return s.str();
		}
		#endif
	};
}