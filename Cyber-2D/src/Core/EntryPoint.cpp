#include <pch.h>
#include "Cyber.h"

//defined in the client
extern Cyber::Application* CreateAplication();

int main(int argc,char** argv) {
	Cyber::Log::Init();
	CB_CORE_INFO("STARTING THE ENGINE");

	Cyber::Application* App = CreateAplication();
	App->Run();
	delete App;
}