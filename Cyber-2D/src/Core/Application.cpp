#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW\glfw3.h"
#include "glad\glad.h"


namespace Cyber {
	float mouseX = 0, mouseY = 0;
	bool mousePressed = false;
	Application::Application() {
		m_Window = new Window(WindowProps("TEST", 400, 400));
		m_Window->SetEventCallback([this](const Event* e) {
			CB_CORE_TRACE(*e);
			switch (e->Type)
			{
			case EventType::WindowClose: {
				onWindowClose();
				break;
			}
			case EventType::WindowResize: {
				const WindowResizeEvent* ev = dynamic_cast<const WindowResizeEvent*>(e);
				glViewport(0, 0, ev->width, ev->height);
				m_Window->onUpdate();
				break;
			}
			case EventType::MouseMoved: {
				const MouseMovedEvent* ev = dynamic_cast<const MouseMovedEvent*>(e);
				mouseX = ev->x;
				mouseY = ev->y;
				break;
			}
			case EventType::MouseButtonPressed:
				mousePressed = true;
				break;
			case EventType::MouseButtonReleased:
				mousePressed = false;
				break;
			default:
				break;
			}
			delete e;
			});


		m_Runnig = true;

	}
	Application::~Application() {
		delete m_Window;
	}

	void Application::Run() {
		while (m_Runnig) {
			if (mousePressed)
			{
				glClearColor(mouseX / m_Window->GetWidth(), mouseY / m_Window->GetHeight(), 1, 1);
			}
			else
				glClearColor(mouseX / m_Window->GetWidth(), mouseY / m_Window->GetHeight(), 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
		};
	}

	bool Application::onWindowClose() {
		m_Runnig = false;
		return true;
	}
}