#pragma once
#include "pch.h"
#include "OpenGL\OpenGLContext.h"
#include "Events\Events.h"
#include "Events\WindowEvents.h"
#include "Events\KeyboardEvents.h"
#include "Events\MouseEvents.h"


namespace Cyber {
	using EventCallbackFn = std::function<void(Event*)>;
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Vsync;

		WindowProps(const std::string& title = "Cyber Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool vsync=true)
			: Title(title), Width(width), Height(height),Vsync(vsync)
		{
		}
		
	};
	struct WindowData
	{
		WindowProps props;
		EventCallbackFn onEvent;
		WindowData(EventCallbackFn onEvent_ = [](Event *e) {}, WindowProps props_ = WindowProps()) :
			props(props_),onEvent(onEvent_){}
	};
	class Window {
	public:
		void onUpdate();

		Window(const WindowProps& props = WindowProps());
		~Window();

		uint32_t GetWidth()const { return m_Data.props.Width; };
		uint32_t GetHeight()const { return m_Data.props.Height; };
		void SetEventCallback(const EventCallbackFn& callback);
		void SetVSync(bool enabled);
		bool GetVSync() { return m_Data.props.Vsync; };
		bool IsVSync() const { return m_Data.props.Vsync; }
		GLFWwindow* GetNativeWindow() const { return m_Window; }

		void Init(WindowProps props);
		void Shutdown();
	private:
		WindowData m_Data;
		GLFWwindow* m_Window;
		std::unique_ptr<OpenGLContext> m_Context;
	};
}