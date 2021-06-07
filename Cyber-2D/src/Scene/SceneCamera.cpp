#include "pch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cyber {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_Size = size;
		m_Near = nearClip;
		m_Far = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width,height };
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	
	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_Size * m_AspectRatio * 0.5f;
		float orthoRight = m_Size * m_AspectRatio * 0.5f;
		float orthoBottom = -m_Size * 0.5f;
		float orthoTop = m_Size * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_Near, m_Far);
	}

}
