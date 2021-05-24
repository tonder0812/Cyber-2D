#include "Cyber.h"
#include "EditorLayer.h"
#include "glm\glm.hpp"

namespace Cyber {
	class Editor : public Application
	{
	public:
		Editor(int argc, char** argv)
			:Application(argc, argv, "Cyber Editor")
		{
			CB_CORE_INFO("STARTING EDITOR");
			pushLayer(new EditorLayer());
		}
		~Editor() {

		}
	private:
	};
}
Cyber::Application* CreateAplication(int argc, char** argv) {
	return new Cyber::Editor(argc, argv);
}
