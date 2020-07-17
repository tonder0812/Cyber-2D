#pragma once
#include "pch.h"
#include <GLFW/glfw3.h>
#include "OpenGL\OpenGLContext.h"

namespace Cyber {
	struct Event{};
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Vsync;

		WindowProps(const std::string& title = "Hazel Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool vsync=true)
			: Title(title), Width(width), Height(height),Vsync(vsync)
		{
		}
	};
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		void onUpdate();

		Window(const WindowProps& props = WindowProps());
		~Window();

		uint32_t GetWidth()const { return m_Props.Width; };
		uint32_t GetHeight()const { return m_Props.Height; };
		void SetEventCallback(const EventCallbackFn& callback);
		void SetVSync(bool enabled);
		bool IsVSync() const { return m_Props.Vsync; }
		GLFWwindow* GetNativeWindow() const { return m_Window; }

		void Init(WindowProps props);
		void Shutdown();
	private:
		WindowProps m_Props;
		GLFWwindow* m_Window;
		std::unique_ptr<OpenGLContext> m_Context;
	};
}