#include "pch.h"
#include "OpenGLRenderer.h"
#include "OpenGLErrorCallback.h"
#include "glad/glad.h"

namespace Cyber {
	void Renderer::SetClearColor(glm::vec4 color)
	{
		GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
	}
	void Renderer::Clear()
	{
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		GL_CHECK(glViewport(0, 0, width, height));
	}
	void Renderer::DrawIndexed(VertexBuffer* vb, IndexBuffer* ib)
	{
		vb->Bind(true);
		ib->Bind();
		uint32_t count = ib->GetCount();
		GL_CHECK(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}
}