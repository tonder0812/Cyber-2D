#include "Cyber.h"
#include "ExampleLayer.h"
#include "glm\glm.hpp"


class Sandbox : public Cyber::Application
{
public:
	Sandbox(){
		//Application();
		name = "Sandbox";
		CB_INFO("STARTING SANDBOX");
		pushLayer(new DemoLayer());
	}
	~Sandbox() {

	}
private:
};

Cyber::Application* CreateAplication() {
	return new Sandbox;
}
