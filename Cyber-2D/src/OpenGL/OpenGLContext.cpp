#include "pch.h"
#include "Core/Log.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cyber {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		if (!windowHandle)
			CB_CORE_ERROR("Window handle is null!");
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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}