#include "StandAloneLayer.h"
#include "OpenGL/OpenGLRenderer.h"
#include "Scene/SceneSerializer.h"

namespace Cyber {
	void StandAloneLayer::onAttach()
	{
		m_CurrentScene = new Scene(true);
		SceneSerializer ss = SceneSerializer(*m_CurrentScene);
		ss.Deserialize("./index.cyber");
		Window* window = Application::Get().GetWindow();
		m_ViewportSize = { window->GetWidth(),window->GetHeight() };
	}
	void StandAloneLayer::onDetach()
	{
		delete m_CurrentScene;
	}
	void StandAloneLayer::onImGUI() {}

	void StandAloneLayer::onUpdate(float ts)
	{
		if (m_ViewportSizeChanged) {
			m_CurrentScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ViewportSizeChanged = false;
		}
		Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
		Renderer::Clear();
		m_CurrentScene->OnUpdateRuntime(ts);
	}
	bool StandAloneLayer::onEvent(const Event* e)
	{
		if (e->Type == EventType::WindowResize) {
			WindowResizeEvent* ev = (WindowResizeEvent*)e;
			m_ViewportSize = { ev->width,ev->height };
			m_ViewportSizeChanged = true;
		}
		return false;
	}
}