#pragma once
#include "Window.h"
#include "Layer.h"
#include "ImGUILayer.h"

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
		const std::vector<std::string> GetArgs() { return m_args; };
	private:
		static Application* s_Instance;
		Window* m_Window;
		LayerStack* m_LayerStack;
		bool m_Runnig;
		bool m_Minimized = false;
		bool onWindowClose();
		ImGUILayer* m_ImGuiLayer;
		float lastFrameTime = 0;
		std::vector<std::string> m_args;
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Application& app) {
	os << app.name;
	return os;
}