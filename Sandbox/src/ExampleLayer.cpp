#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ExampleLayer.h"
#include "Core/Application.h"
#include "OpenGL/OpenGLRenderer.h"

void DemoLayer::onAttach() {
	int width = Cyber::Application::Get().GetWindow()->GetWidth();
	int height = Cyber::Application::Get().GetWindow()->GetHeight();
	float vert[] = {
		-width / 2.0f , -height / 2.0f , 0,
		 width / 2.0f , -height / 2.0f , 0,
		 width / 2.0f ,  height / 2.0f , 0,
		-width / 2.0f ,  height / 2.0f , 0
	};
	m_VertexBuff = new Cyber::VertexBuffer(vert, sizeof(vert));

	Cyber::BufferLayout Layout = {
		{Cyber::ShaderDataType::Float3,"a_Position"}
	};
	Layout.Bind();
	m_VertexBuff->SetLayout(Layout);
	uint32_t indices[] = { 0,1,2,2,3,0 };
	m_IndexBuff = new Cyber::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
	m_Shader = new Cyber::Shader("assets/shaders/example.shader");
}

void DemoLayer::onDetach() {
	delete m_VertexBuff;
	delete m_IndexBuff;
	delete m_Shader;
}

void DemoLayer::onImGUI() {
	ImGui::ColorEdit3("Rectangle Color", glm::value_ptr(m_Color));
	ImGui::Checkbox("Use Color", &m_useColor);
	//ImGui::ShowDemoWindow();
}
void DemoLayer::onUpdate(float ts) {
	Cyber::Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
	Cyber::Renderer::Clear();
	uint32_t width = Cyber::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Cyber::Application::Get().GetWindow()->GetHeight();
	float vert[] = {
		-(int)width / 2.0f , -(int)height / 2.0f , 0,
		 (int)width / 2.0f , -(int)height / 2.0f , 0,
		 (int)width / 2.0f ,  (int)height / 2.0f , 0,
		-(int)width / 2.0f ,  (int)height / 2.0f , 0
	};
	m_VertexBuff->SetData(vert, sizeof(vert));
	if (Cyber::Input::IsKeyPressed(CB_KEY_UP))
		cameraPos.y -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_DOWN))
		cameraPos.y += cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_RIGHT))
		cameraPos.x -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_LEFT))
		cameraPos.x += cameraSpeed * ts;

	m_Camera = glm::translate(glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, -1.0f, 1.0f), cameraPos);
	m_Shader->Bind();
	m_Shader->UploadUniformFloat3("u_color", m_Color);
	m_Shader->UploadUniformInt("u_useColor", m_useColor ? 1 : 0);
	m_Shader->UploadUniformMat4("u_camera", m_Camera);
	Cyber::Renderer::DrawIndexed(m_VertexBuff, m_IndexBuff);
}

bool DemoLayer::onEvent(const Cyber::Event* e) {
	if (e->Type == Cyber::EventType::KeyPressed)
	{
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
			break;
		default:
			break;
		}
	}
	return false;
}