#include "Cyber.h"
#include "EditorLayer.h"
#include "glm\glm.hpp"

namespace Cyber {
	class Editor : public Application
	{
	public:
		Editor() {
			name = "Cyber Editor";
			CB_INFO("STARTING EDITOR");
			pushLayer(new EditorLayer());
		}
		~Editor() {

		}
	private:
	};
}
Cyber::Application* CreateAplication() {
	return new Cyber::Editor;
}
