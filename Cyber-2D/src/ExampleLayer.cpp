#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include "ExampleLayer.h"
#include "Core\Application.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void DemoLayer::onAttach() {
	float vert[] = {
		-190.0f , -190.0f , 0,
		 190.0f , -190.0f , 0,
		 190.0f ,  190.0f , 0,
		-190.0f ,  190.0f , 0
	};
	m_VertexBuff = new Cyber::VertexBuffer(vert, sizeof(vert));
	Cyber::BufferLayout Layout = {
		{Cyber::ShaderDataType::Float3,"a_Position"}
	};
	Layout.Bind();
	m_VertexBuff->SetLayout(Layout);
	uint32_t indices[] = { 0,1,2,2,3,0 };
	m_IndexBuff = new Cyber::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));

	std::string vertexSource = R"(
		#version 330 core

		layout(location=0) in vec3 a_pos;
		uniform mat4 u_camera;

		void main(){
			gl_Position=u_camera*vec4(a_pos,1.0);
		}
	)";
	std::string fragmentSource = R"(
		#version 330 core

		layout(location=0) out vec4 color;	
		uniform vec3 u_color;		

		void main(){
			color=vec4(u_color,1.0);
			//color=vec4(1.0,0.0,0.0,1.0);
		}
	)";

	m_Shader = new Cyber::Shader("Example Shader", vertexSource.c_str(), fragmentSource.c_str());
}

void DemoLayer::onDetach() {
	delete m_VertexBuff;
	delete m_IndexBuff;
	delete m_Shader;
}

void DemoLayer::onImGUI() {
	ImGui::ColorEdit3("Rectangle Color", glm::value_ptr(m_Color));
	//ImGui::ShowDemoWindow();
}
void DemoLayer::onUpdate() {
	if (Cyber::Input::IsKeyPressed(CB_KEY_UP))
		cameraPos.y -= cameraSpeed;
	if (Cyber::Input::IsKeyPressed(CB_KEY_DOWN))
		cameraPos.y += cameraSpeed;
	if (Cyber::Input::IsKeyPressed(CB_KEY_RIGHT))
		cameraPos.x -= cameraSpeed;
	if (Cyber::Input::IsKeyPressed(CB_KEY_LEFT))
		cameraPos.x += cameraSpeed;
	uint32_t width = Cyber::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Cyber::Application::Get().GetWindow()->GetHeight();
	m_Camera = glm::translate(glm::ortho(-(width / 2.0f), width / 2.0f, -(height / 2.0f), height / 2.0f, -1.0f, 1.0f), cameraPos);
	m_Shader->Bind();
	m_Shader->UploadUniformFloat3("u_color", m_Color);
	m_Shader->UploadUniformMat4("u_camera", m_Camera);
	m_VertexBuff->Bind();
	m_IndexBuff->Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuff->GetCount(), GL_UNSIGNED_INT, nullptr);
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
		default:
			break;
		}
	}
	return false;
}