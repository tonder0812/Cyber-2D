#include "Cyber.h"

class Sandbox : public Cyber::Application
{
public:
	Sandbox(){
		name = "Sandbox";
		CB_INFO("STARTING SANDBOX");
	}
	~Sandbox() {

	}
private:
};

Cyber::Application* CreateAplication() {
	return new Sandbox;
}
