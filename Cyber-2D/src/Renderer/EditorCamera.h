#pragma once

#include "Camera.h"
#include "Events/Events.h"
#include "Events/MouseEvents.h"

#include <glm/glm.hpp>

namespace Cyber {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float size, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float ts);
		bool OnEvent(const Event* e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetNear() { return m_NearClip; }
		float GetFar() { return m_FarClip; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(const MouseScrolledEvent* e);

		void MousePan(const glm::vec2& delta);
		void MouseZoom(float delta);

		float ZoomSpeed() const;
	private:
		float m_Size = 10.0f, m_AspectRatio = (16.0f / 9.0f), m_NearClip = -1.0f, m_FarClip = 1.0f;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}
