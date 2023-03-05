#include "pkpch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Log.h"
#include "PKEngine/Events/ApplicationEvent.h"
#include "Input.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


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

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3*7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
			 0.0f,  0.5f, 0.0f,	1.0f,1.0f,0.0f,1.0f
		};

		uint32_t indices[3] = {
			0,1,2
		};
		
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices,sizeof(vertices)));
		
		BufferLayout layout = {
				{ShaderDataType::Float3,"a_Position"},
				{ShaderDataType::Float4,"a_Color",true}
			};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		
		m_SqureVA.reset(VertexArray::Create());

		float squreVertices[4 * 3] = {
	-0.5f, -0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f, 
	 0.5f,  0.5f, 0.0f,	
	 -0.5f, 0.5f, 0.0f
		};

		uint32_t squreIndices[6] = {
			0,1,2,
			2,3,0
		};

		std::shared_ptr<VertexBuffer> squreVB;
		squreVB.reset(VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		BufferLayout squreLayout = {
		{ShaderDataType::Float3,"a_Position"}
		};
		squreVB->SetLayout(squreLayout);
		m_SqureVA->AddVertexBuffer(squreVB);

		std::shared_ptr<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		m_SqureVA->SetIndexBuffer(squreIB);
		
		std::string vertexS = R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 a_Color;
			out vec4 v_Color;
			void main()
			{
			gl_Position =  position;
			v_Color = a_Color;
			}; )";

		std::string fragS = R"(#version 330 core
			out vec4 color;
			in vec4 v_Color;
			void main()
			{
			color = v_Color;//vec4(0.8f,0.3f,0.2f,1.0f);
			};)";

		std::string vertexS2 = R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			
			void main()
			{
			gl_Position =  position;
			}; )";

		std::string fragS2 = R"(#version 330 core
			out vec4 color;
			
			void main()
			{
			color = vec4(0.8f,0.3f,0.2f,1.0f);
			};)";

		m_Shader.reset(new Shader(vertexS, fragS));
		m_SqureShader.reset(new Shader(vertexS2,fragS2));
	}
	
	Application::~Application() {

	}

	void Application::Run() {
		//WindowResizeEvent e(1280, 720);
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			m_SqureShader->Bind();
			m_SqureVA->Bind();
			GLCALL(glDrawElements(GL_TRIANGLES, m_SqureVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));

			m_Shader->Bind();
			m_VertexArray->Bind();
			GLCALL(glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
			

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