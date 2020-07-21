#include "pch.h"
#include "Cyber.h"
#include <imgui.h>
#include "ExampleLayer.h"
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>


void DemoLayer::onImGUI() {
	ImGui::ColorEdit3("Background Color", glm::value_ptr(m_Color));
}
void DemoLayer::onUpdate() {
	glClearColor(m_Color[0], m_Color[1], m_Color[2], 1);
	glClear(GL_COLOR_BUFFER_BIT);
}