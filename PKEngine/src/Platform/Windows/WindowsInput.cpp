#include "pkpch.h"
#include "WindowsInput.h"

#include "PKEngine/Application.h"
#include <GLFW/glfw3.h>
namespace PKEngine {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double px, py;
		glfwGetCursorPos(window, &px, &py);
		return { (float)px, (float)py };
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto v = GetMousePositionImpl();
		return std::get<0>(v);
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto v = GetMousePositionImpl();
		return std::get<1>(v);
	}
}