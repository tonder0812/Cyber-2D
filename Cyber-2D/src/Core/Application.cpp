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
		if (s_Instance!=nullptr)
		{
			CB_CORE_CRITICAL("Aplication Already Open");
		}
		s_Instance = this;
		m_LayerStack = LayerStack(this);
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
			/*
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
				break;*/
			default:
				break;
			}
			m_LayerStack.onEvent(e);
			delete e;
			});
		CB_CORE_TRACE("BEFORE IMGUI");
		m_ImGuiLayer = new ImGUILayer();
		CB_CORE_TRACE("MID IMGUI");
		m_LayerStack.pushLayer(m_ImGuiLayer);
		CB_CORE_TRACE("AFTER IMGUI");
		m_Runnig = true;

	}
	Application::~Application() {
		m_LayerStack.popLayer(m_ImGuiLayer);
		delete m_Window;
	}

	void Application::Run() {
		CB_CORE_TRACE("RUN");
		while (m_Runnig) {
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