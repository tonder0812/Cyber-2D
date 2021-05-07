#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "OpenGL/OpenGLRenderer.h"
#include "ImGUILayer.h"
#include "Python/Python.h"
extern "C" PyObject * PyInit_Cyber(void);

namespace Cyber {
	float mouseX = 0, mouseY = 0;
	bool mousePressed = false;

	Application* Application::s_Instance = nullptr;
	Application::Application(int argc, char** argv) {
		if (s_Instance != nullptr)
		{
			CB_CORE_CRITICAL("Aplication Already Open");
		}

		//decode argv
		wchar_t** _argv = new wchar_t* [argc];
		for (int i = 0; i < argc; i++) {
			_argv[i] = Py_DecodeLocale(argv[i], NULL);
			m_args.emplace_back(argv[i]);
			CB_CORE_TRACE(argv[i]);
		}

		//set program name
		Py_SetProgramName(_argv[0]);
		//add cyber module
		Py_Import_Module(Cyber);
		//start python interpreter
		Py_Initialize();
		//set sys.argv
		PySys_SetArgv(argc, _argv);

		s_Instance = this;
		m_LayerStack = new LayerStack();
		m_Window = new Window(WindowProps(1900, 980, "TEST", true, m_Minimized));
		//m_Window = new Window(WindowProps("TEST",m_Minimized));
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
				if (ev->width == 0 || ev->height == 0)
				{
					m_Minimized = true;
					break;
				}
				m_Minimized = false;
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
		Renderer::Init();
		m_ImGuiLayer = new ImGUILayer();
		m_LayerStack->pushOverlay(m_ImGuiLayer);
		m_Runnig = true;
	}
	Application::~Application() {
		m_LayerStack->popOverlay(m_ImGuiLayer);
		Renderer::Shutdown();
		Py_FinalizeEx();
		delete m_LayerStack;
		delete m_Window;
	}

	void Application::Run() {
		CB_CORE_TRACE("RUN");
		while (m_Runnig) {
			float now = (float)glfwGetTime();
			float ts = now - lastFrameTime;
			lastFrameTime = now;
			//CB_CORE_TRACE("Frame rate: {0:.2f}fps", 1 / ts);
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