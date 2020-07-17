#pragma once

struct GLFWwindow;
namespace Cyber {

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();
	private:
		GLFWwindow* m_WindowHandle;
	};

}

