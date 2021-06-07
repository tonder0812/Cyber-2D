#pragma once

#include "Renderer/Camera.h"

namespace Cyber {

	class SceneCamera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetSize() const { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }
		float GetNearClip() const { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }
		float GetFarClip() const { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }
		float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; RecalculateProjection(); }

		glm::vec2 Normalize(glm::vec2 point) {
			point.x /= m_ViewportSize.x / 2.0f;
			point.y /= m_ViewportSize.y / 2.0f;
			point.x -= 1;
			point.y -= 1;
			point.y *= -1;
			return point;
		}
		const glm::mat4& GetProjection() const { return m_Projection; }

	private:
		void RecalculateProjection();
	private:

		float m_Size = 10.0f;
		float m_Near = -1.0f, m_Far = 1.0f;
		glm::vec2 m_ViewportSize;
		float m_AspectRatio = 0.0f;

		glm::mat4 m_Projection;
	};

}
