#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "OpenGL/OpenGLRenderer.h"
#include "ImGUILayer.h"
#include "Python/Python.h"
#include "Python/Utils.h"
#include <imgui.h>
extern "C" PyObject * PyInit_Cyber(void);
extern "C" PyObject * PyInit_glm(void);

namespace Cyber {
	float mouseX = 0, mouseY = 0;
	bool mousePressed = false;

	Application* Application::s_Instance = nullptr;
	Application::Application(int argc, char** argv, const std::string& name) {
		CB_CORE_ASSERT(!s_Instance, "Aplication Already Open");
		this->name = name;

		//decode argv
		wchar_t** _argv = new wchar_t* [argc];
		for (int i = 0; i < argc; i++) {
			_argv[i] = Py_DecodeLocale(argv[i], NULL);
			m_args.emplace_back(argv[i]);
			CB_CORE_TRACE(argv[i]);
		}

		//set path
		std::filesystem::path exePath = m_args[0];
		m_Path = exePath.parent_path();
		if (m_Path.is_relative()) {
			m_Path = std::filesystem::canonical(m_Path);
		}
		CB_CORE_TRACE(m_Path.string());
		std::wstring path = m_Path.wstring();
		std::wstringstream ss;
		ss << L".;";
		ss << path << L"\\packages\\python.3.9.4\\tools;";
		ss << path << L"\\packages\\python.3.9.4\\tools\\Lib;";
		ss << path << L"\\packages\\python.3.9.4\\tools\\DLLs;";
		ss << path << L"\\packages\\python.3.9.4\\tools\\Lib\\site-packages;";
		ss << m_Path.parent_path().parent_path().parent_path().wstring() << L"\\packages\\python.3.9.4\\tools;";
		ss << m_Path.parent_path().parent_path().parent_path().wstring() << L"\\packages\\python.3.9.4\\tools\\Lib;";
		ss << m_Path.parent_path().parent_path().parent_path().wstring() << L"\\packages\\python.3.9.4\\tools\\DLLs;";
		ss << m_Path.parent_path().parent_path().parent_path().wstring() << L"\\packages\\python.3.9.4\\tools\\Lib\\site-packages;";
		Py_SetPath(ss.str().c_str());

		//set program name
		Py_SetProgramName(_argv[0]);
		//add cyber module
		Py_Import_Module(Cyber);
		Py_Import_Module(glm);
		//start python interpreter
		Py_Initialize();
		//set sys.argv
		PySys_SetArgv(argc, _argv);
		//add assets/scripts to search path
		PyObject* sysPath = PySys_GetObject("path");
		PyList_Append(sysPath, PyUnicode_FromString("./assets/scripts"));

		//save PyGLM and Cyber to save on importing it every time
		m_PyGLM = PyImport_ImportModule("glm");
		m_PyGLM_Vec2 = PythonUtils::GetFuncFromModule(m_PyGLM, "vec2");
		m_PyGLM_Vec3 = PythonUtils::GetFuncFromModule(m_PyGLM, "vec3");
		m_PyGLM_Vec4 = PythonUtils::GetFuncFromModule(m_PyGLM, "vec4");
		m_PyGLM_Mat4 = PythonUtils::GetFuncFromModule(m_PyGLM, "mat4");

		m_PyCyber = PyImport_ImportModule("Cyber");
		m_PyCyber_Texture = PythonUtils::GetFuncFromModule(m_PyCyber, "Texture");
		m_PyCyber_Entity = PythonUtils::GetFuncFromModule(m_PyCyber, "Entity");
		m_PyCyber_Transform = PythonUtils::GetFuncFromModule(m_PyCyber, "TransformComponent");
		m_PyCyber_SpriteRenderer = PythonUtils::GetFuncFromModule(m_PyCyber, "SpriteRendererComponent");
		m_PyCyber_Camera = PythonUtils::GetFuncFromModule(m_PyCyber, "CameraComponent");
		m_PyCyber_Script = PythonUtils::GetFuncFromModule(m_PyCyber, "ScriptComponent");


		s_Instance = this;
		m_LayerStack = new LayerStack();
		m_Window = new Window(WindowProps(1900, 980, name, true, m_Minimized));
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
			case EventType::KeyPressed: {
				const KeyPressedEvent* ev = dynamic_cast<const KeyPressedEvent*>(e);
				bool consumed = false;
				for (const Hotkey& hotkey : m_Hotkeys) {
					if (hotkey.key == ev->key && hotkey.mods == ev->mods) {
						consumed |= hotkey.func();
					}
				}
				if (consumed) {
					delete e;
					return;
				}
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
		delete m_LayerStack;
		delete m_Window;
		Py_FinalizeEx();
	}

	void Application::Run() {
		CB_CORE_TRACE("RUN");
		while (m_Runnig) {
			float now = (float)glfwGetTime();
			float ts = now - lastFrameTime;
			lastFrameTime = now;
			m_LayerStack->onUpdate(ts);

			ImGuiIO& io = ImGui::GetIO();
			io.FontGlobalScale = m_fontSize / 40.0f;
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