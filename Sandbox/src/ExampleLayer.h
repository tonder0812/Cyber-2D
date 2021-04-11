#pragma once
#include <glm\glm.hpp>
#include "OpenGL\OpenGLBuffer.h"
#include "OpenGL\OpenGLShader.h"
#include "OpenGL\OpenGLTexture.h"
#include "Renderer\OrthographicCamera.h"

class DemoLayer : public Cyber::Layer {
public:
	DemoLayer() :
		Layer("Example Layer"), m_VertexBuff(nullptr), m_IndexBuff(nullptr), m_Shader(nullptr),m_Camera(-1,1,-1,1) {
	};
	void onAttach() override;
	void onDetach() override;
	void onImGUI() override;
	void onUpdate(float ts) override;
	bool onEvent(const Cyber::Event* e) override;
private:
	glm::vec3 m_Color = { 1.0f,0,0 };
	Cyber::OrthographicCamera m_Camera;
	Cyber::VertexBuffer* m_VertexBuff = nullptr;
	Cyber::IndexBuffer* m_IndexBuff = nullptr;
	Cyber::Shader* m_Shader = nullptr;
	Cyber::Texture* m_Texture = nullptr;
	bool m_useColor = false;
	bool m_useImage = false;
	bool m_ignoreNext = false;
	float scale = 1;
	char m_TextureLocation[255]="assets/textures/cic.png";
	glm::vec3 cameraPos = { 0,0,0 };
	float cameraSpeed = 500.0f;
};