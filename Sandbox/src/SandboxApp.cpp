#include <PKEngine.h>

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		PushOverlay(new PKEngine::ImGuiLayer());
	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
