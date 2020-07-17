#include <pch.h>
#include "Cyber.h"

//defined in the client
extern Cyber::Application* CreateAplication();

int main(int argc, char** argv) {
	Cyber::Log::Init();
	CB_CORE_INFO("STARTING THE ENGINE");
	Cyber::Application* App = CreateAplication();
	CB_CORE_INFO("Started Aplication {0}", *App);
	App->Run();
	delete App;
}