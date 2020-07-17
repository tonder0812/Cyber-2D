#include "pch.h"
#include "Cyber.h"

class Sandbox : public Cyber::Application
{
public:
	Sandbox(){
		CB_INFO("STARTING SANDBOX");
	}
	~Sandbox() {

	}
	void Run() {
		CB_TRACE("Run");
		while (true);
	}
private:
};

Cyber::Application* CreateAplication() {
	return new Sandbox;
}
