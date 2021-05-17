#include "pch.h"
#include "EditorCamera.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cyber {

	EditorCamera::EditorCamera(float size, float aspectRatio, float nearClip, float farClip)
		: m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Size(size)
	{
		UpdateProjection();
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		float orthoLeft = -m_Size * m_AspectRatio * 0.5f;
		float orthoRight = m_Size * m_AspectRatio * 0.5f;
		float orthoBottom = -m_Size * 0.5f;
		float orthoTop = m_Size * 0.5f;
		m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void EditorCamera::UpdateView()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Size * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance;//* distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(float ts)
	{
		const glm::vec2& mouse=Input::GetMousePosition();
		glm::vec2 delta = (mouse - m_InitialMousePosition);
		m_InitialMousePosition = mouse;
		if (Input::IsMouseButtonPressed(CB_MOUSE_BUTTON_RIGHT) || (Input::IsKeyPressed(CB_KEY_LEFT_ALT) && Input::IsMouseButtonPressed(CB_MOUSE_BUTTON_LEFT)))
			MousePan(delta);

		UpdateView();
	}

	bool EditorCamera::OnEvent(const Event* e)
	{
		if (e->Type == EventType::MouseScrolled) {
			return OnMouseScroll((MouseScrolledEvent*)e);
		}
		return false;
	}

	bool EditorCamera::OnMouseScroll(const MouseScrolledEvent* e)
	{
		float delta = e->yoff * 0.1f;
		MouseZoom(delta);
		UpdateProjection();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		m_Position.x -= (delta.x / m_ViewportWidth) * m_Size * m_AspectRatio;
		m_Position.y += (delta.y / m_ViewportHeight) * m_Size;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Size -= delta * ZoomSpeed();
	}

}
