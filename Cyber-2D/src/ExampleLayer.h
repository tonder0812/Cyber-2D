#pragma once
#include <glm\glm.hpp>

class DemoLayer : public Cyber::Layer {
public:
	DemoLayer() :
		Layer("Demo") {};
	void onImGUI() override;
	void onUpdate();
private:
	glm::vec3 m_Color = { 0,0,0 };
};