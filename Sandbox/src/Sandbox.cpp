#include "pch.h"
#include "Cyber.h"

class Sandbox : public Cyber::Application
{
public:
	Sandbox(){
		#ifdef CB_DEBUG
		name = "Sandbox";
		#endif
		CB_INFO("STARTING SANDBOX");
	}
	~Sandbox() {

	}
private:
};

Cyber::Application* CreateAplication() {
	return new Sandbox;
}
