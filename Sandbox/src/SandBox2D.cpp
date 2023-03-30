#include "SandBox2D.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name,Fn func) :m_Name(name),m_Func(func)
	{
		m_Stop = false;
		m_StartPoint = std::chrono::high_resolution_clock::now();
		//m_Func = func;

	}
	~Timer() 
	{
		if (!m_Stop)
		{
			Stop();
		}
	}

	void Stop() 
	{
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now()).time_since_epoch().count();

		float during = (end - start)* 0.001f;
		//std::cout << m_Name << ":" << during << "ms" << std::endl;
		m_Stop = true;
		m_Func({m_Name,during});
	}
	

private:
	const char* m_Name;
	std::chrono::steady_clock::time_point m_StartPoint;
	bool m_Stop;
	Fn m_Func;
};


SandBox2D::SandBox2D()
	:Layer("SandBox2D"),m_CameraController(1920.0f / 1080.0f), m_SqureColor(1.0f)
{
}

void SandBox2D::OnAttach()
{
	m_SqureVA = PKEngine::VertexArray::Create();

	float squreVertices[] = {
-0.5f, -0.5f, 0.0f, 0.0f,0.0f,
 0.5f, -0.5f, 0.0f,	1.0f,0.0f,
 0.5f,  0.5f, 0.0f,	1.0f,1.0f,
 -0.5f, 0.5f, 0.0f,	0.0f,1.0f
	};

	uint32_t squreIndices[6] = {
		0,1,2,
		2,3,0
	};

	PKEngine::Ref<PKEngine::VertexBuffer> squreVB;
	squreVB.reset(PKEngine::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
	PKEngine::BufferLayout squreLayout = {
	{PKEngine::ShaderDataType::Float3,"a_Position"},
	{PKEngine::ShaderDataType::Float2,"a_TexCoord"}
	};
	squreVB->SetLayout(squreLayout);
	m_SqureVA->AddVertexBuffer(squreVB);

	PKEngine::Ref<PKEngine::IndexBuffer> squreIB;
	squreIB.reset(PKEngine::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
	m_SqureVA->SetIndexBuffer(squreIB);

	m_SqureShader = PKEngine::Shader::Create("assets/shaders/squareShader.glsl");

	m_Texture = PKEngine::Texture2D::Create("assets/textures/emotion1.png");
}

void SandBox2D::OnDetach()
{
}

#define PROFILE_STAT(name) Timer timer##__Line__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); });

void SandBox2D::OnUpdate(PKEngine::Timestep ts)
{
	PROFILE_STAT("SandBox2D OnUpdate");

	//update
	{
		PROFILE_STAT("CameraController Update");
		m_CameraController.Update(ts);
	}
	//rotate vector by this
	//glm::quat rrot = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::quat quat(0,glm::vec3(1.0f,0.0f,0.0f));
	//auto m = glm::vec4(1.0f,0.0f,0,0) * rrot;
	//PK_INFO("({0},{1},{2})", m.x, m.y, m.z);

	//render
	{
		PROFILE_STAT("Renderer Pre");
		PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		PKEngine::RenderCommand::Clear();
		PKEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{
		PROFILE_STAT("Renderer Draw");
		PKEngine::Renderer2D::DrawQuad(glm::vec2(0.0f,0.0f), glm::vec2(1.0f,1.0f), glm::vec4(1.0f));
		PKEngine::Renderer2D::DrawQuad(glm::vec2(1.0f,1.0f), glm::vec2(0.5f,0.5f), glm::vec4(0.8f,0.3f,0.2f,1.0f));
		PKEngine::Renderer2D::DrawQuad(glm::vec3(1.0f,1.0f,-0.1f), glm::vec2(10.0f,10.0f), m_Texture);
	}
	//PKEngine::Renderer2D::EndScene();

	//m_SqureShader->Bind();
//std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform4f("u_Color", m_SqureColor);

//auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
//PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader, transform);

}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqureColor));

	for (auto& profileResult : m_ProfileResults) 
	{
		char label[50];
		strcpy(label, profileResult.name);
		strcat(label, ":%.3f");
		ImGui::Text(label, profileResult.time);
	}
	ImGui::End();
	m_ProfileResults.clear();
}

void SandBox2D::OnEvent(PKEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
