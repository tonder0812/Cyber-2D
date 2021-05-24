#include "Cyber.h"
#include "ExampleLayer.h"
#include "StandAloneLayer.h"
#include "glm\glm.hpp"


class Sandbox : public Cyber::Application
{
public:
	Sandbox(int argc, char** argv) :
		Application(argc, argv, "Cyber Engine") {
		CB_CORE_INFO("STARTING SANDBOX");
		//pushLayer(new DemoLayer());
		pushLayer(new Cyber::StandAloneLayer());
	}
	~Sandbox() {

	}
private:
};

Cyber::Application* CreateAplication(int argc, char** argv) {
	return new Sandbox(argc, argv);
}
