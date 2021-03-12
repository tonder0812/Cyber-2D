#include "pch.h"
#include "Core/Log.h"
#include "OpenGLContext.h"
#include "OpenGLErrorCallback.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cyber {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CB_CORE_ASSERT(windowHandle,"Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if(!status)
			CB_CORE_ERROR("Failed to initialize Glad!");

		CB_CORE_INFO("OpenGL Info:");
		CB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		uint32_t va;
		GL_CHECK(glGenVertexArrays(1, &va));
		GL_CHECK(glBindVertexArray(va));
		CB_CORE_WARN("GL Version {0}.{1}", versionMajor, versionMinor);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}