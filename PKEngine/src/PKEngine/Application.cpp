#include "pkpch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Log.h"
#include "PKEngine/Events/ApplicationEvent.h"
#include "Input.h"

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

	Application::Application() {
		PK_CORE_ASSERT(!s_Instance, "Application is already exist!");
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		s_Instance = this;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		float vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		unsigned int indices[3] = {
			0,1,2
		};

		GLCALL(glGenVertexArrays(1, &m_VertexArray));
		GLCALL(glBindVertexArray(m_VertexArray));

		GLCALL(glGenBuffers(1, &m_VertexBuffer));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW));
		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

		GLCALL(glGenBuffers(1, &m_IndexBuffer));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	
		//m_Shader = glCreateProgram();
		//const char* vertexS = R"(
		//		#version 330 core
		//		layout(location = 0) in vec3 a_Position;
		//		void main()
		//		{
		//			gl_Position =  a_Position;
		//		};
		//	)";

		std::string vertexS = R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			void main()
			{
			gl_Position =  position;
			}; )";

		std::string fragS = R"(#version 330 core
			out vec4 color;
			void main()
			{
			color = vec4(0.8f,0.3f,0.2f,1.0f);
			};)";

		m_Shader.reset(new Shader(vertexS,fragS));

		//unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
		//glShaderSource(vShader, 1, &vertexS, nullptr);
		//glCompileShader(vShader);

		//unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
		//glShaderSource(fShader, 1, &fragS, nullptr);
		//glCompileShader(fShader);

		//glAttachShader(m_Shader, vShader);
		//glAttachShader(m_Shader, fShader);
		//glLinkProgram(m_Shader);
		//glValidateProgram(m_Shader);
	}
	
	Application::~Application() {

	}

	void Application::Run() {
		//WindowResizeEvent e(1280, 720);
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//glUseProgram(m_Shader);
			m_Shader->Bind();
			GLCALL(glBindVertexArray(m_VertexArray));
			GLCALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));


			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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
}