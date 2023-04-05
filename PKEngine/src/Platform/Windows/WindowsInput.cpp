#include "pkpch.h"
#include "PKEngine/Core/Input.h"

#include "PKEngine/Core/Application.h"
#include <GLFW/glfw3.h>
namespace PKEngine {
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double px, py;
		glfwGetCursorPos(window, &px, &py);
		return { (float)px, (float)py };
	}
	float Input::GetMouseX()
	{
		auto v = GetMousePosition();
		return std::get<0>(v);
	}
	float Input::GetMouseY()
	{
		auto v = GetMousePosition();
		return std::get<1>(v);
	}
}