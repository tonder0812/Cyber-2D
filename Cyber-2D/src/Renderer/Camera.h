#pragma once

#include <glm/glm.hpp>

namespace Cyber {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection, const glm::mat4& view)
			: m_ProjectionMatrix(projection), m_ViewMatrix(view) 
		{
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		virtual ~Camera() = default;
		void SetProjectionMatrix(const glm::mat4& projection) {
			m_ProjectionMatrix = projection;
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		void SetViewMatrix(const glm::mat4& view) {
			m_ViewMatrix = view;
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}