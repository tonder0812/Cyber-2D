#include "pch.h"
#include "Window.h"
#include <GLFW/glfw3.h>


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
		m_Data.props = props;
		CB_CORE_INFO("New window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (s_WindowCount == 0)
		{
			int success = glfwInit();
			if (!success)
				CB_CORE_ERROR("Failed to initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			#ifdef CB_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif


		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.props.Title.c_str(), nullptr, nullptr);
		++s_WindowCount;

		m_Context = std::make_unique<OpenGLContext>(OpenGLContext(m_Window));
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


		//###############GLFW CALLBACKS
		//WINDOW CALLBACKS
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.props.Width = width;
				data.props.Height = height;
				WindowResizeEvent* e = new WindowResizeEvent(width, height);
				data.onEvent(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent* e = new WindowCloseEvent();
				data.onEvent(e);
			});

		glfwSetWindowPosCallback(m_Window,[](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowMovedEvent* e = new WindowMovedEvent(xpos,ypos);
				data.onEvent(e);
			});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window,int focus)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				Event* e;
				if (focus)
				{
					e = new WindowFocusEvent();
				}
				else {
					e = new WindowLostFocusEvent();
				}
				data.onEvent(e);
			});


		//KEYBORD CALLBACKS
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				Event* e;
				switch (action)
				{
				case GLFW_PRESS:
				{
					e = new KeyPressedEvent(key,mods, false);
					break;
				}
				case GLFW_RELEASE:
				{
					e = new KeyReleasedEvent(key,mods);
					break;
				}
				case GLFW_REPEAT:
				{
					e = new KeyPressedEvent(key,mods, true);
					break;
				}
				}
				data.onEvent(e);
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent* e = new KeyTypedEvent(keycode);
				data.onEvent(e);
			});


		//MOUSE CALLBACKS
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				Event* e;
				switch (action)
				{
				case GLFW_PRESS:
				{
					e = new MousePressedEvent(button);
					break;
				}
				case GLFW_RELEASE:
				{
					e = new MouseReleasedEvent(button);
					break;
				}
				}
				data.onEvent(e);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent* e = new MouseScrolledEvent(xOffset, yOffset);
				data.onEvent(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent* e = new MouseMovedEvent(xPos,yPos);
				data.onEvent(e);
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

		m_Data.props.Vsync = enabled;
	}

	void Window::SetEventCallback(const EventCallbackFn& callback) { m_Data.onEvent = callback; }
}