#pragma once
#include "Window.h"
#include "Layer.h"
#include "ImGUILayer.h"

namespace Cyber {
	class LayerStack;
	class Application {
	public:
		Application();
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
	private:
		static Application* s_Instance;
		Window* m_Window;
		LayerStack m_LayerStack;
		bool m_Runnig;
		bool onWindowClose();
		ImGUILayer* m_ImGuiLayer;
		float lastFrameTime = 0;
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Application& app) {
	os << app.name;
	return os;
}