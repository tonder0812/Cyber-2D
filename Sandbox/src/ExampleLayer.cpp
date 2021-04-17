#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ExampleLayer.h"
#include "Core/Application.h"
#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLTexture.h"
#include "OpenGL/OpenGLFrameBuffer.h"

void DemoLayer::onAttach() {


	Cyber::FramebufferSpecification fbSpec;
	fbSpec.Attachments = { Cyber::FramebufferTextureFormat::RGBA8, Cyber::FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = new Cyber::Framebuffer(fbSpec);

	m_Texture = new Cyber::Texture(m_TextureLocation);
	int width = Cyber::Application::Get().GetWindow()->GetWidth();
	int height = Cyber::Application::Get().GetWindow()->GetHeight();
	CB_CORE_INFO("{0}:{1}", m_Texture->GetWidth(), m_Texture->GetHeight());

	m_Texture->Bind();
	m_Camera = Cyber::OrthographicCamera(-(width / 2.0f) / scale, width / 2.0f / scale, -(height / 2.0f) / scale, height / 2.0f / scale);
}

void DemoLayer::onDetach() {
	delete m_Texture;
	delete m_Framebuffer;
}

void DemoLayer::onImGUI() {

	StartDockspace();

	ImGui::Begin("test");
	ImGui::ColorEdit4("Rectangle Color", glm::value_ptr(m_Color));
	if (ImGui::Checkbox("Use Color", &m_useColor) && m_useColor) {
		m_useTint = false;
	}
	if (ImGui::Checkbox("Use Tint", &m_useTint) && m_useTint) {
		m_useColor = false;
	}
	ImGui::InputText("Texture location", m_TextureLocation, sizeof(m_TextureLocation));
	if (ImGui::Button("Update")) {
		delete m_Texture;
		m_Texture = new Cyber::Texture(m_TextureLocation);
	}
	ImGui::End();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), viewportPanelSize, { 0, 1 }, { 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}
void DemoLayer::onUpdate(float ts) {
	if (Cyber::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Camera.SetProjection(-(m_ViewportSize.x / 2.0f) / scale, m_ViewportSize.x / 2.0f / scale, -(m_ViewportSize.y / 2.0f) / scale, m_ViewportSize.y / 2.0f / scale);
	}
	m_Framebuffer->Bind();
	Cyber::Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
	Cyber::Renderer::Clear();
	uint32_t width = Cyber::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Cyber::Application::Get().GetWindow()->GetHeight();
	//if (!ImGui::GetIO().WantCaptureKeyboard) {
	if (Cyber::Input::IsKeyPressed(CB_KEY_UP))
		cameraPos.y -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_DOWN))
		cameraPos.y += cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_RIGHT))
		cameraPos.x -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_LEFT))
		cameraPos.x += cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_A))
		m_Camera.SetRotation(m_Camera.GetRotation() + 0.2 * ts);
	if (Cyber::Input::IsKeyPressed(CB_KEY_D))
		m_Camera.SetRotation(m_Camera.GetRotation() - 0.2 * ts);
	m_Camera.SetPosition(cameraPos);
	//}
	Cyber::Renderer::BeginScene(m_Camera);
	if (m_useColor)
	{
		Cyber::Renderer::DrawQuad({ 0,0 }, { m_Texture->GetWidth(),m_Texture->GetHeight() }, m_Color);
	}
	else if (m_useTint)
	{
		Cyber::Renderer::DrawQuad({ 0,0 }, { m_Texture->GetWidth(),m_Texture->GetHeight() }, m_Texture, 1, m_Color);
	}
	else {
		Cyber::Renderer::DrawQuad({ 0,0 }, { m_Texture->GetWidth(),m_Texture->GetHeight() }, m_Texture);
	}
	Cyber::Renderer::EndScene();
	m_Framebuffer->Unbind();
}

bool DemoLayer::onEvent(const Cyber::Event* e) {
	switch (e->Type)
	{
	case Cyber::EventType::KeyPressed:
	{
		//if (ImGui::GetIO().WantCaptureKeyboard) return true;
		Cyber::KeyPressedEvent* ev = (Cyber::KeyPressedEvent*)e;
		switch (ev->key)
		{
		case CB_KEY_ESCAPE:
			Close();
			break;
		case CB_KEY_E:
			Cyber::Application::Get().GetWindow()->SetVSync(false);
			break;
		case CB_KEY_Q:
			Cyber::Application::Get().GetWindow()->SetVSync(true);
			break;
		case CB_KEY_T:
			Cyber::Application::Get().GetWindow()->SetVSync(!Cyber::Application::Get().GetWindow()->GetVSync());
			break;
		case CB_KEY_R:
			cameraPos = { 0,0,0 };
			scale = 1;
			m_Camera.SetRotation(0);
			break;
		default:
			break;
		}
		break;
	}
	case Cyber::EventType::MouseScrolled:
	{
		Cyber::MouseScrolledEvent* ev = (Cyber::MouseScrolledEvent*)e;
		scale += ev->yoff * 0.1;
		if (scale < 0.1)
		{
			scale = 0.1;
		}
		m_Camera.SetProjection(-(m_ViewportSize.x / 2.0f) / scale, m_ViewportSize.x / 2.0f / scale, -(m_ViewportSize.y / 2.0f) / scale, m_ViewportSize.y / 2.0f / scale);
		break;
	}
	case Cyber::EventType::WindowResize:
	{
		int width = Cyber::Application::Get().GetWindow()->GetWidth();
		int height = Cyber::Application::Get().GetWindow()->GetHeight();
		m_Camera.SetProjection(-(width / 2.0f) / scale, width / 2.0f / scale, -(height / 2.0f) / scale, height / 2.0f / scale);
		break;
	}
	}
	return false;
}


void DemoLayer::StartDockspace() {
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