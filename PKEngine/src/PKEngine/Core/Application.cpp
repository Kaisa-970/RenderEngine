#include "pkpch.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Log.h"
#include "PKEngine/Events/ApplicationEvent.h"
#include "Input.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "PKEngine/Renderer/Renderer.h"
#include <PKEngine/Core/Timestep.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) GLClearError(); \
x; \
ASSERT(GLCheck(__FILE__,#x,__LINE__));


static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLCheck(const char* filename, const char* function, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error {" << error << "}\t" << function << " " << filename << " " << line << std::endl;
		return false;
	}
	return true;
}

namespace PKEngine {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		PK_CORE_ASSERT(!s_Instance, "Application is already exist!");
		WindowProps wp;
		wp.Title = name;
		wp.Width = width;
		wp.Height = height;
		m_Window = std::unique_ptr<Window>(Window::Create(wp));
		
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		s_Instance = this;
		
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Minimize = false;
	}
	
	Application::~Application() {

	}

	void Application::Run() {
		//WindowResizeEvent e(1280, 720);
		while (m_Running) {
			float time = glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimize) {
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(ts);
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	void Application::Close()
	{
		m_Running = false;
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimize = true;
		}
		m_Minimize = false;

		Renderer::SetViewport(e.GetWidth(), e.GetHeight());
		return false;
	}
}