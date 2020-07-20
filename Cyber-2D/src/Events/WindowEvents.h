#pragma once
#include "Events.h"

namespace Cyber {
	struct WindowCloseEvent : public Event
	{
		WindowCloseEvent() {
			Type = EventType::WindowClose;
		};
		
		std::string getName() const override {
			return "Window Close Event";
		}
		
	};

	struct WindowResizeEvent : public Event
	{
		unsigned int width = 0, height = 0;
		WindowResizeEvent(unsigned int width_ = 0, unsigned int height_ = 0) :
			width(width_), height(height_) {
			Type = EventType::WindowResize;
		};
		
		std::string getName() const override {
			std::stringstream s;
			s << "Window Resize Event Width:" << width << " Height:" << height;
			return s.str();
		}
		
	};

	struct WindowFocusEvent : public Event
	{
		WindowFocusEvent() {
			Type = EventType::WindowFocus;
		};
		
		std::string getName() const override {
			return "Window Focus Event";
		}
		
	};

	struct WindowLostFocusEvent : public Event
	{
		WindowLostFocusEvent() {
			Type = EventType::WindowLostFocus;
		};
		
		std::string getName() const override {
			return "Window Lost Focus Event";
		}
		
	};

	struct WindowMovedEvent : public Event
	{
		unsigned int x = 0, y = 0;
		WindowMovedEvent(unsigned int x_ = 0, unsigned int y_ = 0) :
			x(x_), y(y_) {
			Type = EventType::WindowMoved;
		};
		
		std::string getName() const override {
			std::stringstream s;
			s << "Window Moved Event X:" << x << " Y:" << y;
			return s.str();
		}
		
	};
}