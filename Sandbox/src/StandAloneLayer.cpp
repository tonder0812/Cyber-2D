#include "StandAloneLayer.h"
#include "OpenGL/OpenGLRenderer.h"
#include "Scene/SceneSerializer.h"
#include "imgui/imgui.h"

namespace Cyber {
	void StandAloneLayer::onAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth
		};
		fbSpec.Width = 1;
		fbSpec.Height = 1;
		m_Framebuffer = new Framebuffer(fbSpec);
		m_CurrentScene = new Scene(true);
		SceneSerializer ss = SceneSerializer(*m_CurrentScene);
		ss.Deserialize("./index.cyber");
	}
	void StandAloneLayer::onDetach()
	{
		delete m_CurrentScene;
		delete m_Framebuffer;
	}
	void StandAloneLayer::onImGUI() {
		bool True = true;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("GameViewport", &True, ImGuiWindowFlags_NoDecoration);
		ImGui::PopStyleVar(3);
		if (viewport->Size.x / 1.7777777 < viewport->Size.y) {
			m_ViewportSize = { viewport->Size.x, viewport->Size.x / 1.7777777 };
		}
		else {
			m_ViewportSize = { viewport->Size.y * 1.7777777, viewport->Size.y };
		}
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 cursor = {
			(viewportPanelSize.x - m_ViewportSize.x) * 0.5f,
			(viewportPanelSize.y - m_ViewportSize.y) * 0.5f
		};
		ImGui::SetCursorPos(cursor);

		glm::vec4 bounds = { cursor.x,cursor.y, cursor.x + m_ViewportSize.x, cursor.y + m_ViewportSize.y };
		Input::SetBounds(bounds);
		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), *(ImVec2*)&m_ViewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
	}

	void StandAloneLayer::onUpdate(float ts)
	{
		//if (m_ViewportSizeChanged) {
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CurrentScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ViewportSizeChanged = false;
		}
		m_Framebuffer->Bind();
		Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
		Renderer::Clear();
		m_CurrentScene->OnUpdateRuntime(ts);
		m_Framebuffer->Unbind();
	}
	bool StandAloneLayer::onEvent(const Event* e)
	{
		/*if (e->Type == EventType::WindowResize) {
			WindowResizeEvent* ev = (WindowResizeEvent*)e;
			m_ViewportSize = { ev->width,ev->height };
			m_ViewportSizeChanged = true;
		}*/
		return false;
	}
}