#pragma once
#include "glm/glm.hpp"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
namespace Cyber {
	class Renderer
	{
	public:
		static void SetClearColor(glm::vec4 color);
		static void Clear();
		static void SetViewport(uint32_t width, uint32_t height);
		static void DrawIndexed(VertexBuffer *vb,IndexBuffer *ib);
	};
}

