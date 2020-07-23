#pragma once
#include <glm\glm.hpp>
#include "OpenGL\OpenGLBuffer.h"
#include "OpenGL\OpenGLShader.h"

class DemoLayer : public Cyber::Layer {
public:
	DemoLayer() :
		Layer("Example Layer") {
	};
	void onAttach() override;
	void onDetach() override;
	void onImGUI() override;
	void onUpdate() override;
private:
	glm::vec3 m_Color = { 1.0f,0,0 };
	unsigned int m_VertexArr;
	Cyber::VertexBuffer* m_VertexBuff;
	Cyber::IndexBuffer* m_IndexBuff;
	Cyber::Shader* m_Shader;
};