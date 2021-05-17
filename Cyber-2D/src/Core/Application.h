#pragma once
#include "Window.h"
#include "Layer.h"
#include "ImGUILayer.h"
#include "Python/Python.h"

namespace Cyber {
	class LayerStack;
	class Application {
	public:
		Application(int argc, char** argv);
		virtual ~Application();
		virtual void Run();
		std::string name;
		bool Close() {
			return onWindowClose();
		}
		Window* GetWindow() { return m_Window; };
		static Application& Get() { return *s_Instance; }
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		int GetFontSyze() { return m_fontSize; };
		void SetFontSyze(int value) { m_fontSize = value; };
		const std::vector<std::string>& GetArgs() { return m_args; };
		PyObject* GetPyGLM() { return m_PyGLM; };
		PyObject* GetPyGLM_Vec2() { return m_PyGLM_Vec2; };
		PyObject* GetPyGLM_Vec3() { return m_PyGLM_Vec3; };
		PyObject* GetPyGLM_Vec4() { return m_PyGLM_Vec4; };
		PyObject* GetPyGLM_Mat4() { return m_PyGLM_Mat4; };
		PyObject* GetPyCyber() { return m_PyCyber; };
		PyObject* GetPyCyber_Transform() { return m_PyCyber_Transform; };
		std::filesystem::path getCWD() { return m_CWD; };
		std::filesystem::path getPath() { return m_Path; };
	private:
		static Application* s_Instance;
		Window* m_Window;
		LayerStack* m_LayerStack;
		bool m_Runnig;
		bool m_Minimized = false;
		bool onWindowClose();
		ImGUILayer* m_ImGuiLayer;
		float lastFrameTime = 0;
		int m_fontSize = 20;
		std::vector<std::string> m_args;
		PyObject* m_PyGLM = nullptr;
		PyObject* m_PyGLM_Vec2 = nullptr;
		PyObject* m_PyGLM_Vec3 = nullptr;
		PyObject* m_PyGLM_Vec4 = nullptr;
		PyObject* m_PyGLM_Mat4 = nullptr;
		PyObject* m_PyCyber = nullptr;
		PyObject* m_PyCyber_Transform = nullptr;
		std::filesystem::path m_CWD;
		std::filesystem::path m_Path;
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Application& app) {
	os << app.name;
	return os;
}