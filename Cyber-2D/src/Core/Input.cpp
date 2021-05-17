#include "pch.h"
#include "Input.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


namespace Cyber {
	static glm::vec4 bounds;
	static bool blockMouseInput = false;
	static bool blockKeyboardInput = false;

	bool Input::IsKeyPressed(int32_t key)
	{
		if (blockKeyboardInput)
			return false;

		GLFWwindow* window = Application::Get().GetWindow()->GetNativeWindow();
		auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int32_t button)
	{
		if (blockMouseInput)
			return false;

		GLFWwindow* window = Application::Get().GetWindow()->GetNativeWindow();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = Application::Get().GetWindow()->GetNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		glm::vec2 pos = GetMousePosition();
		return pos.x;
	}

	float Input::GetMouseY()
	{
		glm::vec2 pos = GetMousePosition();
		return pos.y;
	}

	glm::vec2 Input::GetMousePositionViewport()
	{
		GLFWwindow* window = Application::Get().GetWindow()->GetNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (bounds.x != 0 || bounds.y != 0 || bounds.z != 0 || bounds.w != 0) {
			xpos -= bounds.x;
			ypos -= bounds.y;
		}
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseXViewport()
	{
		glm::vec2 pos = GetMousePositionViewport();
		return pos.x;
	}

	float Input::GetMouseYViewport()
	{
		glm::vec2 pos = GetMousePositionViewport();
		return pos.y;
	}

	void Input::SetBounds(glm::vec4 bounds_) {
		bounds = bounds_;
	}

	bool Input::IsMouseBlocked() {
		return blockMouseInput;
	}

	void Input::SetMouseBlocked(bool value) {
		blockMouseInput = value;
	}

	bool Input::IsKeyboardBlocked() {
		return blockKeyboardInput;
	}

	void Input::SetKeyboardBlocked(bool value) {
		blockKeyboardInput = value;
	}
}
