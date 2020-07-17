#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW\glfw3.h"
#include "glad\glad.h"

namespace Cyber {
	Window* m_Window;
	bool m_Runnig;
	Application::Application() {
		m_Window = new Window(WindowProps("TEST",400,400));
		m_Runnig = true;
	}
	Application::~Application() {
		delete m_Window;
	}

	void Application::Run() {
		while (m_Runnig) {
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
		}
	};
}