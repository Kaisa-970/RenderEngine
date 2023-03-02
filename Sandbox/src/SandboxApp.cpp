#include <PKEngine.h>
#include <imgui.h>

class ExampleLayer : public PKEngine::Layer {
	virtual void OnImGuiRender()override {
		ImGui::Text("123");
	}
};

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		//PushOverlay(new PKEngine::ImGuiLayer());
		//PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
