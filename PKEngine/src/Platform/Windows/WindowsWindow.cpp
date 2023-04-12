#include "pkpch.h"
//#include "glad/glad.h"
#include "WindowsWindow.h"
#include "PKEngine/Core/Log.h"
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/Events/KeyEvent.h"
#include "PKEngine/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace PKEngine {
	static bool s_GLFWInitialied = false;

	static void GLFWErrorCallback(int errorcode, const char* description) {
		PK_CORE_ERROR("GLFW Error ({0}): {1}", errorcode, description);
	}

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
		m_Context->SwapBuffers();
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
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialied = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		auto rendererInfo = glGetString(GL_RENDERER);
		PK_CORE_INFO("Renderer:{0}", (const char*)rendererInfo);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent e(width, height);
			data.Width = width;
			data.Height = height;

			data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.EventCallback(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key,int scancode,int action,int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) 
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);

					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);

					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);

					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window,unsigned int c) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(c);
			data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent e(button);
						data.EventCallback(e);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent e(button);
						data.EventCallback(e);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window,double xoffset,double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xoffset, (float)yoffset);
				data.EventCallback(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent e((float)xpos, (float)ypos);
				data.EventCallback(e);
			});
	}

	void PKEngine::WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

