#pragma once

#ifdef PK_PLATFORM_WINDOWS

//extern PKEngine::Application* PKEngine::CreateApplication();

int main(int argc,char** argv) {
	PKEngine::Log::Init();
	PK_CORE_WARN("Initialized!");
	PK_INFO("Hello!Var = {0}",5);

	//if (!glfwInit()) {
	//	return -1;
	//}
	//auto window = glfwCreateWindow(1280, 760, "Window", NULL,NULL);

	

	auto* app = PKEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // PK_PLATFORM_WINDOWS
