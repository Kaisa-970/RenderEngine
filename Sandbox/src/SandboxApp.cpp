#include <PKEngine.h>

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		
	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
