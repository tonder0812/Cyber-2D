#pragma once
#include "glm/glm.hpp"

namespace Cyber {
	struct Input {
		static bool IsKeyPressed(int32_t key);
		static bool IsMouseButtonPressed(int32_t button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePositionViewport();
		static float GetMouseXViewport();
		static float GetMouseYViewport();
		static void SetBounds(glm::vec4 bounds);
		static bool Input::IsMouseBlocked();
		static void Input::SetMouseBlocked(bool value);
		static bool Input::IsKeyboardBlocked();
		static void Input::SetKeyboardBlocked(bool value);
	};
}