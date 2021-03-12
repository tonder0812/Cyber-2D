#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "OpenGL/OpenGLRenderer.h"
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
		m_LayerStack = new LayerStack();
		m_Window = new Window(WindowProps(400, 400, "TEST"));
		//m_Window = new Window(WindowProps("TEST"));
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
				Renderer::SetViewport(ev->width, ev->height);
				/*m_LayerStack.onUpdate();
				m_ImGuiLayer->Begin();
				m_LayerStack.onImGUI();
				m_ImGuiLayer->End();
				m_Window->onUpdate();*/
				break;
			}
			default:
				break;
			}
			m_LayerStack->onEvent(e);
			delete e;
			});
		m_ImGuiLayer = new ImGUILayer();
		m_LayerStack->pushOverlay(m_ImGuiLayer);
		m_Runnig = true;

	}
	Application::~Application() {
		m_LayerStack->popOverlay(m_ImGuiLayer);
		delete m_LayerStack;
		delete m_Window;
	}

	void Application::Run() {
		CB_CORE_TRACE("RUN");
		while (m_Runnig) {
			float now = glfwGetTime();
			float ts = now - lastFrameTime;
			lastFrameTime = now;
			CB_CORE_TRACE("Frame rate: {0:.2f}fps", 1 / ts);
			m_LayerStack->onUpdate(ts);
			m_ImGuiLayer->Begin();
			m_LayerStack->onImGUI();
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
		m_LayerStack->pushLayer(layer);
	}
	void Application::popLayer(Layer* layer) {
		m_LayerStack->popLayer(layer);
	}
}