#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ExampleLayer.h"
#include "Core/Application.h"
#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGLTexture.h"

void DemoLayer::onAttach() {
	/*m_Texture = new Cyber::Texture(19, 19);
	glm::u8vec4 data[19 * 19];
	for (size_t x = 0; x < 19; x++)
	{
		for (size_t y = 0; y < 19; y++)
		{
			int v = ((x * 19 + y) % 2 == 0) ? 255 : 0;
			data[x * 19 + y] = glm::u8vec4(v, v, v, 255);
		}
	}
	m_Texture->SetData(data, sizeof(data));
	*/
	m_Texture = new Cyber::Texture("assets/textures/cic.png");
	int width = Cyber::Application::Get().GetWindow()->GetWidth();
	int height = Cyber::Application::Get().GetWindow()->GetHeight();
	/*float vert[] = {
		-width / 2.0f , -height / 2.0f , 0,  0 , 0,
		 width / 2.0f , -height / 2.0f , 0,  1 , 0,
		 width / 2.0f ,  height / 2.0f , 0,  1 , 1,
		-width / 2.0f ,  height / 2.0f , 0,  0 , 1
	};*/
	CB_CORE_INFO("{0}:{1}", m_Texture->GetWidth(), m_Texture->GetHeight());
	float vert[] = {
		-(m_Texture->GetWidth() / 2.0f) , -(m_Texture->GetHeight() / 2.0f) , 0,  0 , 0,
		 (m_Texture->GetWidth() / 2.0f) , -(m_Texture->GetHeight() / 2.0f) , 0,  1 , 0,
		 (m_Texture->GetWidth() / 2.0f) ,  (m_Texture->GetHeight() / 2.0f) , 0,  1 , 1,
		-(m_Texture->GetWidth() / 2.0f) ,  (m_Texture->GetHeight() / 2.0f) , 0,  0 , 1
	};
	m_VertexBuff = new Cyber::VertexBuffer(vert, sizeof(vert));

	Cyber::BufferLayout Layout = {
		{Cyber::ShaderDataType::Float3,"a_Position"},
		{Cyber::ShaderDataType::Float2,"a_TexCoord"}
	};
	Layout.Bind();
	m_VertexBuff->SetLayout(Layout);
	uint32_t indices[] = { 0,1,2,2,3,0 };
	m_IndexBuff = new Cyber::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
	m_Texture->Bind();
	m_Shader = new Cyber::Shader("assets/shaders/example.shader");
	m_Shader->Bind();
	m_Shader->UploadUniformInt("u_sampler", 0);
	m_Shader->UploadUniformInt("u_channels", m_Texture->GetChannels());
}

void DemoLayer::onDetach() {
	delete m_VertexBuff;
	delete m_IndexBuff;
	delete m_Shader;
	delete m_Texture;
}

void DemoLayer::onImGUI() {
	ImGui::ColorEdit3("Rectangle Color", glm::value_ptr(m_Color));
	if (ImGui::Checkbox("Use Color", &m_useColor) && m_useColor) {
		m_useImage = false;
	}
	if (ImGui::Checkbox("Use Image", &m_useImage) && m_useImage) {
		m_useColor = false;
	}
	if (ImGui::InputText("Texture location", m_TextureLocation, sizeof(m_TextureLocation))) {
		m_ignoreNext = true;
	}
	if (ImGui::Button("Update")) {
		delete m_Texture;
		m_Texture = new Cyber::Texture(m_TextureLocation);
		int width = Cyber::Application::Get().GetWindow()->GetWidth();
		int height = Cyber::Application::Get().GetWindow()->GetHeight();
		CB_CORE_INFO("{0}:{1}", m_Texture->GetWidth(), m_Texture->GetHeight());
		float vert[] = {
			-(m_Texture->GetWidth() / 2.0f) , -(m_Texture->GetHeight() / 2.0f) , 0,  0 , 0,
			 (m_Texture->GetWidth() / 2.0f) , -(m_Texture->GetHeight() / 2.0f) , 0,  1 , 0,
			 (m_Texture->GetWidth() / 2.0f) ,  (m_Texture->GetHeight() / 2.0f) , 0,  1 , 1,
			-(m_Texture->GetWidth() / 2.0f) ,  (m_Texture->GetHeight() / 2.0f) , 0,  0 , 1
		};
		m_VertexBuff->SetData(vert, sizeof(vert));
		m_Texture->Bind();
		m_Shader->Bind();
		m_Shader->UploadUniformInt("u_channels", m_Texture->GetChannels());

	}
	//ImGui::ShowDemoWindow();
}
void DemoLayer::onUpdate(float ts) {
	CB_CORE_TRACE(scale);
	Cyber::Renderer::SetClearColor({ 0.1,0.1,0.1,1 });
	Cyber::Renderer::Clear();
	uint32_t width = Cyber::Application::Get().GetWindow()->GetWidth();
	uint32_t height = Cyber::Application::Get().GetWindow()->GetHeight();
	/*float vert[] = {
		-width / 2.0f , -height / 2.0f , 0,  0 , 0,
		 width / 2.0f , -height / 2.0f , 0,  1 , 0,
		 width / 2.0f ,  height / 2.0f , 0,  1 , 1,
		-width / 2.0f ,  height / 2.0f , 0,  0 , 1
	};
	m_VertexBuff->SetData(vert, sizeof(vert));*/
	if (Cyber::Input::IsKeyPressed(CB_KEY_UP))
		cameraPos.y -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_DOWN))
		cameraPos.y += cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_RIGHT))
		cameraPos.x -= cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_LEFT))
		cameraPos.x += cameraSpeed * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_A))
		rotation += 0.2 * ts;
	if (Cyber::Input::IsKeyPressed(CB_KEY_D))
		rotation -= 0.2 * ts;

	m_Camera = glm::translate(glm::rotate(glm::ortho(-(width / 2.0f) / scale, width / 2.0f / scale, -(height / 2.0f) / scale, height / 2.0f / scale, -1.0f, 1.0f), rotation, { 0,0,1 }), cameraPos);

	m_Shader->UploadUniformFloat3("u_color", m_Color);
	m_Shader->UploadUniformInt("u_useColor", m_useColor ? 1 : 0);
	m_Shader->UploadUniformInt("u_useImage", m_useImage ? 1 : 0);
	m_Texture->Bind();
	m_Shader->Bind();
	m_Shader->UploadUniformMat4("u_camera", m_Camera);
	Cyber::Renderer::DrawIndexed(m_VertexBuff, m_IndexBuff);
}

bool DemoLayer::onEvent(const Cyber::Event* e) {
	if (e->Type == Cyber::EventType::KeyPressed)
	{
		Cyber::KeyPressedEvent* ev = (Cyber::KeyPressedEvent*)e;
		if (m_ignoreNext) {
			m_ignoreNext = false;
			return true;
		}
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
			rotation = 0;
			break;
		default:
			break;
		}
	}
	else if (e->Type == Cyber::EventType::MouseScrolled)
	{
		Cyber::MouseScrolledEvent* ev = (Cyber::MouseScrolledEvent*)e;
		scale += ev->yoff * 0.1;
		if (scale < 0.1)
		{
			scale = 0.1;
		}
	}
	return false;
}