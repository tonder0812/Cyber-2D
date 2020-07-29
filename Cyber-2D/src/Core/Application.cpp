#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW\glfw3.h"
#include "glad\glad.h"
#include "ImGUILayer.h"

namespace Cyber {
	float mouseX = 0, mouseY = 0;
	bool mousePressed = false;
	Application* Application::s_Instance = nullptr;
	Application::Application() {
		if (s_Instance != nullptr)
		{
			CB_CORE_CRITICAL("Aplication Already Open");
		}
		s_Instance = this;
		m_LayerStack = LayerStack();
		//m_Window = new Window(WindowProps(400, 400, "TEST"));
		m_Window = new Window(WindowProps("TEST"));
		m_Window->SetEventCallback([this](const Event* e) {
			//CB_CORE_TRACE(*e);
			switch (e->Type)
			{
			case EventType::WindowClose: {
				onWindowClose();
				break;
			}
			case EventType::WindowResize: {
				const WindowResizeEvent* ev = dynamic_cast<const WindowResizeEvent*>(e);
				glViewport(0, 0, ev->width, ev->height);
				m_LayerStack.onUpdate();
				m_ImGuiLayer->Begin();
				m_LayerStack.onImGUI();
				m_ImGuiLayer->End();
				m_Window->onUpdate();
				break;
			}
			default:
				break;
			}
			m_LayerStack.onEvent(e);
			delete e;
			});
		m_ImGuiLayer = new ImGUILayer();
		m_LayerStack.pushOverlay(m_ImGuiLayer);
		m_Runnig = true;

	}
	Application::~Application() {
		m_LayerStack.popOverlay(m_ImGuiLayer);
		delete m_Window;
	}

	void Application::Run() {
		CB_CORE_TRACE("RUN");
		while (m_Runnig) {
			CB_CORE_TRACE("Time since start: {0}s", glfwGetTime());
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			/*if (mousePressed)
			{
				glClearColor(mouseX / m_Window->GetWidth(), mouseY / m_Window->GetHeight(), 1, 1);
			}
			else
				glClearColor(mouseX / m_Window->GetWidth(), mouseY / m_Window->GetHeight(), 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);*/
			m_LayerStack.onUpdate();
			m_ImGuiLayer->Begin();
			m_LayerStack.onImGUI();
			m_ImGuiLayer->End();

			m_Window->onUpdate();
		};
		CB_CORE_TRACE("CLOSE");
	}

	bool Application::onWindowClose() {
		m_Runnig = false;
		return true;
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
	}
	void Application::popLayer(Layer* layer) {
		m_LayerStack.popLayer(layer);
	}
}