#pragma once

namespace Cyber {
	struct Input {
		static bool IsKeyPressed(int32_t key);
		static bool IsMouseButtonPressed(int32_t button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}