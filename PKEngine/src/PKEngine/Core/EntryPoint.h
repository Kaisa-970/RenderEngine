#pragma once

#ifdef PK_PLATFORM_WINDOWS

//extern PKEngine::Application* PKEngine::CreateApplication();

int main(int argc,char** argv) {
	PKEngine::Log::Init();

	PK_PROFILE_BEGIN_SESSION("Startup", "PKEngineProfile-Startup.json");
	auto* app = PKEngine::CreateApplication();
	PK_PROFILE_END_SESSION();

	PK_PROFILE_BEGIN_SESSION("Shutdown", "PKEngineProfile-Runtime.json");
	app->Run();
	PK_PROFILE_END_SESSION();

	PK_PROFILE_BEGIN_SESSION("Startup", "PKEngineProfile-Shutdown.json");
	delete app;
	PK_PROFILE_END_SESSION();
}

#endif // PK_PLATFORM_WINDOWS
