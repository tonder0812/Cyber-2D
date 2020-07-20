#pragma once
#include "Window.h"


namespace Cyber {
	class Application {
	public:
		Application();
		virtual ~Application();
		virtual void Run();
		#ifdef CB_DEBUG
		std::string name;
		#endif

	private:
		Window* m_Window;
		bool m_Runnig;
		bool onWindowClose();
	};
}

inline std::ostream& operator<< (std::ostream& os, const Cyber::Application& app) {
	#ifdef CB_DEBUG
	os << app.name;
	#else
	os << "Cyber Aplication";
	#endif
	return os;
}