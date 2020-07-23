#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include "ExampleLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void DemoLayer::onAttach() {
	float vert[] = {
		-0.5f , -0.5f , 0,
		 0.5f , -0.5f , 0,
		 0.5f ,  0.5f , 0,
		-0.5f ,  0.5f , 0
	};
	m_VertexBuff = new Cyber::VertexBuffer(vert,sizeof(vert));
	Cyber::BufferLayout Layout = {
		{Cyber::ShaderDataType::Float3,"a_Position"}
	};
	Layout.Bind();
	m_VertexBuff->SetLayout(Layout);
	uint32_t indices[] = { 0,1,2,2,3,0 };
	m_IndexBuff = new Cyber::IndexBuffer(indices, sizeof(indices)/sizeof(uint32_t));


	std::string vertexSource = R"(
		#version 330 core

		layout(location=0) in vec3 a_pos;

		void main(){
			gl_Position=vec4(a_pos,1.0);
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
	m_Shader->Bind();
	m_Shader->UploadUniformFloat3("u_color", m_Color);
	m_VertexBuff->Bind();
	m_IndexBuff->Bind();
	glDrawElements(GL_TRIANGLES, m_IndexBuff->GetCount(), GL_UNSIGNED_INT, nullptr);
}