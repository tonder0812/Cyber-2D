#include <pch.h>
#include "Application.h"

//defined in the client
extern Cyber::Application* CreateAplication(int argc, char** argv);

int main(int argc, char** argv) {
	std::filesystem::path exePath = std::filesystem::path(argv[0]).parent_path();
	if (exePath.is_relative()) {
		exePath = std::filesystem::canonical(exePath);
	}
	std::filesystem::current_path(exePath);
	Cyber::Log::Init();
#ifndef CB_CONSOLE
    HWND consoleWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId(consoleWnd, &dwProcessId);
    if (GetCurrentProcessId() == dwProcessId)
    {
	    ShowWindow(consoleWnd, SW_HIDE);
    }
#endif
	CB_CORE_INFO("STARTING THE ENGINE\0");
	Cyber::Application* App = CreateAplication(argc, argv);
	CB_CORE_INFO("Started Aplication {0}", *App);
	App->Run();
	delete App;
#ifdef CB_DEBUG
	std::cin.get();
#endif
}