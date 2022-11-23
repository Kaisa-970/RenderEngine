#pragma once

#ifdef PK_PLATFORM_WINDOWS

//extern PKEngine::Application* PKEngine::CreateApplication();

int main(int argc,char** argv) {
	auto* app = PKEngine::CreateApplication();
	app->Run();

	delete app;
}

#endif // PK_PLATFORM_WINDOWS
