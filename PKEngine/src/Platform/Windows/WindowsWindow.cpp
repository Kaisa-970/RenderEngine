#include "pkpch.h"
#include "WindowsWindow.h"
#include "PKEngine/Log.h"

namespace PKEngine {
	static bool s_GLFWInitialied = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	PKEngine::WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	PKEngine::WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void PKEngine::WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}


	void PKEngine::WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
	}

	void PKEngine::WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void PKEngine::WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		PK_CORE_INFO("Creating window {0} {1}, {2}", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialied) {
			int success = glfwInit();
			
			PK_CORE_ASSERT(success, "Can not initialize GLFW!");
			s_GLFWInitialied = true;
		}
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void PKEngine::WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

