#include "pch.h"
#include "Cyber.h"
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "Core/Application.h"
#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLTexture.h"
#include "OpenGL/OpenGLFrameBuffer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"


namespace Cyber {
	void EditorLayer::onAttach() {
		Cyber::FramebufferSpecification fbSpec;
		fbSpec.Attachments = { Cyber::FramebufferTextureFormat::RGBA8, Cyber::FramebufferTextureFormat::Depth };
		fbSpec.Width = 1;
		fbSpec.Height = 1;
		m_Framebuffer = new Cyber::Framebuffer(fbSpec);
		m_Square = m_CurrentScene.CreateEntity("Square");
		m_Square.AddComponent<SpriteRendererComponent>();
		m_Square.GetComponent<TransformComponent>().Scale = { 300,300 ,1 };
		m_Square.GetComponent<TransformComponent>().Translation.x = -150;
		m_Square2 = m_CurrentScene.CreateEntity("Square2");
		auto texture = std::make_shared<Texture>("./assets/textures/cic.png");
		m_Square2.AddComponent<SpriteRendererComponent>(texture);
		m_Square2.GetComponent<TransformComponent>().Scale = { 300,300 ,1 };
		m_Square2.GetComponent<TransformComponent>().Translation.x = 150;
	}

	void EditorLayer::onDetach() {
		delete m_Framebuffer;
	}

	void EditorLayer::onImGUI() {

		StartDockspace();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		//Viewport
		ImGui::Begin("viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportPanelSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		m_CurrentScene.OnImGui();
		ImGui::End();
	}
	void EditorLayer::onUpdate(float ts) {
		if (Cyber::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetProjection(-(m_ViewportSize.x / 2.0f), m_ViewportSize.x / 2.0f, -(m_ViewportSize.y / 2.0f), m_ViewportSize.y / 2.0f, -100, 100);
		}
		m_Framebuffer->Bind();
		Cyber::Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
		Cyber::Renderer::Clear();
		Renderer::BeginScene(m_Camera);
		m_CurrentScene.OnUpdateEditor(ts);
		Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	bool EditorLayer::onEvent(const Event* e) {
		return false;
	}

	void EditorLayer::StartDockspace() {
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}
}