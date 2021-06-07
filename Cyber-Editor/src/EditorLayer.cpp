#include "pch.h"
#include "Cyber.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ImGuizmo.h>
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
#include "Scene/SceneSerializer.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"

namespace Cyber {
	void EditorLayer::onAttach() {
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth
		};
		fbSpec.Width = 1;
		fbSpec.Height = 1;
		m_Framebuffer = new Framebuffer(fbSpec);
		tmpFontSize = Application::Get().GetFontSyze();
		cameraTexture = std::make_shared<Texture>(Application::Get().getPath().string() + "\\assets\\textures\\camera.png");
		PlayButton = std::make_shared<Texture>(Application::Get().getPath().string() + "\\assets\\textures\\Play.png");
		if (Application::Get().GetArgs().size() > 1) {
			m_CurrentScene = new Scene(true);
			m_CurrentScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_CurrentScene);

			SceneSerializer serializer(*m_CurrentScene);
			serializer.Deserialize(Application::Get().GetArgs()[1]);
		}
		else {
			m_CurrentScene = new Scene();
		}
		m_SceneHierarchyPanel.SetContext(m_CurrentScene);
	}

	void EditorLayer::onDetach() {
		delete m_CurrentScene;
		delete m_Framebuffer;
	}

	void EditorLayer::onImGUI() {
		GImGui->NavWindowingToggleLayer = false;
		StartDockspace();
#ifdef CB_CONSOLE
		ImGui::ShowDemoWindow();
#endif
		bool openSettings = false;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();
				if (ImGui::MenuItem("Settings", "Ctrl+,")) openSettings = true;
				if (ImGui::MenuItem("Exit", "Alt+F4")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		if (openSettings)
			ImGui::OpenPopup("Settings");
		bool True = true;
		ImGui::SetNextWindowSize({ Application::Get().GetWindow()->GetWidth() * 0.5f,Application::Get().GetWindow()->GetHeight() * 0.7f });
		if (ImGui::BeginPopupModal("Settings", &True, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Text("Font");
			ImGui::Indent();
			ImGui::Text("Size");
			ImGui::SameLine();
			ImGui::InputInt("##fontSize", &tmpFontSize);
			ImGui::EndChild();
			ImGui::Text("");

			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Apply  Close  ").x - ImGui::GetStyle().ItemSpacing.x * 2);
			ImGui::CalcItemWidth();
			if (ImGui::Button("Apply")) {
				Application::Get().SetFontSyze(tmpFontSize);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				tmpFontSize = Application::Get().GetFontSyze();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

#ifdef CB_CONSOLE
		ImGui::Begin("Stats");
		ImGui::Text("Framerate: %.2f", framearate);
		ImGui::Text("Framecount: %d", framecount);
		ImGui::Text("Mouse blocked: %d", Input::IsMouseBlocked());
		ImGui::Text("Keyboard blocked: %d", Input::IsKeyboardBlocked());
		ImGui::Text("Hovered id: %d", m_HoveredEntity ? m_HoveredEntity : -1);
		ImGui::Text("Hovered entity: %s", m_HoveredEntity ? (m_HoveredEntity.GetComponent<TagComponent>().Id.c_str()) : "");
		ImGui::End();
#endif

		//Viewport
		ImGui::Begin("Viewport");
		Input::SetMouseBlocked(!ImGui::IsWindowHovered());
		Input::SetKeyboardBlocked(!ImGui::IsWindowFocused());
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportOffset = ImGui::GetWindowPos();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (viewportPanelSize.x / (16.0f/9.0f) < viewportPanelSize.y) {
			m_ViewportSizeScene = { viewportPanelSize.x, viewportPanelSize.x / (16.0f / 9.0f) };
		}
		else {
			m_ViewportSizeScene = { viewportPanelSize.y * (16.0f / 9.0f), viewportPanelSize.y };
		}
		if (m_Runtime)
		{
			m_ViewportSize = m_ViewportSizeScene;
		}
		else {
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		ImVec2 cursor = { (viewportPanelSize.x - m_ViewportSize.x) * 0.5f, (viewportPanelSize.y - m_ViewportSize.y) * 0.5f + viewportMinRegion.y };
		ImGui::SetCursorPos(cursor);
		glm::vec4 bounds = {
			 viewportOffset.x + cursor.x,
			 viewportOffset.y + cursor.y,
			 m_ViewportSize.x + cursor.x + viewportOffset.x,
			 m_ViewportSize.y + cursor.y + viewportOffset.y
		};
		Input::SetBounds(bounds);
		//CB_CORE_INFO("{0}:{1}", (viewportPanelSize.x - m_ViewportSize.x, (viewportPanelSize.y - viewportMinRegion.y - m_ViewportSize.y) / 2.0f + viewportMinRegion.y);
		ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), *(ImVec2*)&m_ViewportSize, { 0, 1 }, { 1, 0 });
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetGizmoSizeClipSpace(0.3);
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(bounds.x, bounds.y, bounds.z - bounds.x, bounds.w - bounds.y);

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(CB_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType & ImGuizmo::OPERATION::ROTATE_Z)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				if (!(translation.x != translation.x ||
					translation.y != translation.y ||
					translation.z != translation.z ||
					rotation.x != rotation.x ||
					rotation.y != rotation.y ||
					rotation.z != rotation.z ||
					scale.x != scale.x ||
					scale.y != scale.y ||
					scale.z != scale.z
					)) {
					float deltaRotation = rotation.z - tc.Transform->Rotation;
					tc.Transform->Translation->super_type = translation;
					tc.Transform->Rotation += deltaRotation;
					tc.Transform->Scale->super_type = scale;
				}
				else {
					CB_CORE_WARN("NAN");
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::Begin("Console");

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		bool clear = false;
		std::pair<std::string*, int> logs = Log::GetContents();
		for (int i = 0; i < logs.second; i++) {
			std::string& line = logs.first[i];
			ImGui::PushID(&logs.first[i]);
			switch (line[0])
			{
			case 'C'://Critical
			case 'E'://Error
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });
				break;
			case 'W'://Warn
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,0,1 });
				break;
			case 'I'://Info
				ImGui::PushStyleColor(ImGuiCol_Text, { 0,1,0,1 });
				break;
			case 'T'://Trace
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1,1 });
				break;
			default:
				ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,1,1 });
				break;
			}
			if (ImGui::Selectable(line.c_str() + 1)) {}
			ImGui::PopStyleColor();
			if (ImGui::BeginPopupContextItem("Item context menu", 1)) {
				if (ImGui::Selectable("Clear")) {
					clear = true;
				}
				if (ImGui::Selectable("Copy")) {
					ImGui::SetClipboardText(line.c_str() + 1);
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ImGui::End();
		if (clear)
			Log::Clear();

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::End();
	}
	void EditorLayer::onUpdate(float ts) {
		framearate = 1 / ts;
		framecount++;

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CurrentScene->OnViewportResize(m_ViewportSizeScene.x, m_ViewportSizeScene.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}
		if (!m_Runtime) {
			m_EditorCamera.OnUpdate(ts);
		}
		m_Framebuffer->Bind();
		Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
		Renderer::Clear();
		m_Framebuffer->ClearAttachment(1, -1);
		if (m_Runtime) {
			m_CurrentScene->OnUpdateRuntime(ts);
		}
		else {
			m_CurrentScene->OnUpdateEditor(ts, m_EditorCamera, cameraTexture);
		}
		glm::vec2 mouse = Input::GetMousePositionViewport();
		if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < m_ViewportSize.x && mouse.y < m_ViewportSize.y) {
			int pixelData = m_Framebuffer->ReadPixel(1, mouse.x, m_ViewportSize.y - mouse.y);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_CurrentScene);
		}
		m_Framebuffer->Unbind();
	}

	bool EditorLayer::onEvent(const Event* e) {
		if (!m_Runtime && m_EditorCamera.OnEvent(e)) return true;
		switch (e->Type)
		{
		case EventType::MouseButtonPressed: {
			MousePressedEvent* ev = (MousePressedEvent*)e;
			if (!m_Runtime && ev->button == CB_MOUSE_BUTTON_LEFT && !ImGuizmo::IsOver() && !Input::IsKeyPressed(CB_KEY_LEFT_ALT)) {
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
				return true;
			}
			break;
		}
		case EventType::KeyPressed: {
			KeyPressedEvent* ev = (KeyPressedEvent*)e;
			switch (ev->key)
			{

				// Gizmos
			case CB_KEY_Q:
				if (!m_Runtime) {
					m_GizmoType = -1;
					return true;
				}
				break;
			case CB_KEY_W:
				if (!m_Runtime) {
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE_X | ImGuizmo::OPERATION::TRANSLATE_Y;
					return true;
				}
				break;
			case CB_KEY_E:
				if (!m_Runtime) {
					m_GizmoType = ImGuizmo::OPERATION::ROTATE_Z;
					return true;
				}
				break;
			case CB_KEY_R:
				if (!m_Runtime) {
					m_GizmoType = ImGuizmo::OPERATION::SCALE_X | ImGuizmo::OPERATION::SCALE_Y;
					return true;
				}
				break;
			}
		}
		}
		return false;
	}
	void EditorLayer::MenuBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		ImGui::Text("");
		const int buttons = 1;
		bool running = m_Runtime;
		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() * 0.5f - buttons * ImGui::GetTextLineHeight() * 0.5f);
		if (running) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		}
		if (ImGui::ImageButton((ImTextureID)PlayButton->GetID(), { ImGui::GetTextLineHeight() * 0.6f,ImGui::GetTextLineHeight() * 0.6f }, { 0,0 }, { 1,1 }, ImGui::GetTextLineHeight() * 0.4f)) {
			if (m_Runtime) {
				m_GizmoType = -1;
				m_HoveredEntity = Entity();
				delete m_CurrentScene;
				m_CurrentScene = new Scene(true);
				SceneSerializer serializer(*m_CurrentScene);
				serializer.Deserialize(Application::Get().getPath().string() + "\\temp\\temp.cyber");

				m_SceneHierarchyPanel.SetContext(m_CurrentScene);
				m_CurrentScene->OnViewportResize((uint32_t)m_ViewportSizeScene.x, (uint32_t)m_ViewportSizeScene.y);
				m_Runtime = false;
			}
			else {
				m_GizmoType = -1;
				m_HoveredEntity = Entity();
				SceneSerializer ss(*m_CurrentScene);
				ss.Serialize(Application::Get().getPath().string() + "\\temp\\temp.cyber");
				m_Runtime = true;
			}
		}
		if (running) {
			ImGui::PopStyleColor(3);
		}
		ImGui::PopStyleVar();
	}

	void EditorLayer::NewScene()
	{
		m_Runtime = false;
		m_CurrentScene = new Scene();
		m_CurrentScene->OnViewportResize((uint32_t)m_ViewportSizeScene.x, (uint32_t)m_ViewportSizeScene.y);
		m_SceneHierarchyPanel.SetContext(m_CurrentScene);
	}

	void EditorLayer::OpenScene()
	{
		//m_Runtime = false;
		std::string filepath = FileDialogs::OpenFile("Scene (*.cyber)\0*.cyber\0");
		if (!filepath.empty())
		{
			delete m_CurrentScene;
			m_CurrentScene = new Scene(true);
			m_CurrentScene->OnViewportResize((uint32_t)m_ViewportSizeScene.x, (uint32_t)m_ViewportSizeScene.y);
			m_SceneHierarchyPanel.SetContext(m_CurrentScene);

			SceneSerializer serializer(*m_CurrentScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Cyber Scene (*.cyber)\0*.cyber\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(*m_CurrentScene);
			serializer.Serialize(filepath);
		}
	}

	void EditorLayer::StartDockspace()
	{
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
		//style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			MenuBar();
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}
}