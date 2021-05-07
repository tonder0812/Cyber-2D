#include "Cyber.h"
#include "ExampleLayer.h"
#include "glm\glm.hpp"


class Sandbox : public Cyber::Application
{
public:
	Sandbox(int argc, char** argv) :
		Application(argc, argv) {
		//Application();
		name = "Sandbox";
		CB_INFO("STARTING SANDBOX");
		pushLayer(new DemoLayer());
	}
	~Sandbox() {

	}
private:
};

Cyber::Application* CreateAplication(int argc, char** argv) {
	return new Sandbox(argc, argv);
}
