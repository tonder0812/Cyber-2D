#pragma once
#include "Events.h"
#include "Core\Keycodes.h"

namespace Cyber {
	struct KeyPressedEvent :public Event {
		bool repeat;
		int key, mods;
		KeyPressedEvent(int key_, int mods_, bool repeat_) :
			key(key_), repeat(repeat_), mods(mods_) {
			Type = EventType::KeyPressed;
		}

		bool ShiftPressed() {
			return mods & CB_MOD_SHIFT;
		}
		bool CtrlPressed() {
			return mods & CB_MOD_CONTROL;
		}
		bool AltPressed() {
			return mods & CB_MOD_ALT;
		}
		bool CapsEnabled() {
			return mods & CB_MOD_CAPS_LOCK;
		}
		bool NumLockEnabled() {
			return mods & CB_MOD_NUM_LOCK;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Key Pressed Event KeyCode: " << key << " Repeat: " << repeat;
			return s.str();
		}
		#endif
	};

	struct KeyReleasedEvent :public Event {
		int key, mods;
		KeyReleasedEvent(int key_, int mods_) :
			key(key_), mods(mods_) {
			Type = EventType::KeyReleased;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Key Released Event KeyCode: " << key;
			return s.str();
		}
		#endif
	};

	struct KeyTypedEvent : public Event {
		int key;
		KeyTypedEvent(int key_) :
			key(key_) {
			Type = EventType::KeyTyped;
		}
		#ifdef CB_DEBUG
		std::string getName()const override {
			std::stringstream s;
			s << "Key Typed Event KeyCode: " << key;
			return s.str();
		}
		#endif
	};
}