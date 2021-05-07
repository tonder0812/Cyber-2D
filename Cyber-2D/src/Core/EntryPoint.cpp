#include <pch.h>
#include "Application.h"

//defined in the client
extern Cyber::Application* CreateAplication(int argc, char** argv);

int main(int argc, char** argv) {
#ifdef CB_CONSOLE
	Cyber::Log::Init();
#else
	FreeConsole();
#endif
	CB_CORE_INFO("STARTING THE ENGINE");
	Cyber::Application* App = CreateAplication(argc, argv);
	CB_CORE_INFO("Started Aplication {0}", *App);
	App->Run();
	delete App;
	std::cin.get();
}