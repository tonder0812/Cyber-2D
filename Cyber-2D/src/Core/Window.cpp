#include "pch.h"
#include "Window.h"
#include "Core\Log.h"

namespace Cyber {
	static uint8_t s_WindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CB_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(WindowProps props) {
		m_Props = props;
		CB_CORE_INFO("New window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (s_WindowCount == 0)
		{
			int success = glfwInit();
			if (!success)
				CB_CORE_ERROR("Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Props.Title.c_str(), nullptr, nullptr);
		++s_WindowCount;

		m_Context = std::make_unique<OpenGLContext>(OpenGLContext(m_Window));
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Props);
		SetVSync(true);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				
			});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_WindowCount;

		if (s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void Window::onUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Props.Vsync = enabled;
	}

}