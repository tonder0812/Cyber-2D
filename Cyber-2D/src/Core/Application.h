#pragma once
#include "Window.h"


namespace Cyber {
	class Application {
	public:
		Application();
		virtual ~Application();
		virtual void Run();
		std::string name;

	private:
		Window* m_Window;
		bool m_Runnig;
		bool onWindowClose();
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Application& app) {
	os << app.name;
	return os;
}