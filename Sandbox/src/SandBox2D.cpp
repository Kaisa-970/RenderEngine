#include "SandBox2D.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <chrono>


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


void SandBox2D::OnUpdate(PKEngine::Timestep ts)
{
	PK_PROFILE_FUNCTION();

	//update
	{
		PK_PROFILE_FUNCTION();
		m_CameraController.Update(ts);
	}
	//rotate vector by this
	//glm::quat rrot = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::quat quat(0,glm::vec3(1.0f,0.0f,0.0f));
	//auto m = glm::vec4(1.0f,0.0f,0,0) * rrot;
	//PK_INFO("({0},{1},{2})", m.x, m.y, m.z);

	//render
	{
		PK_PROFILE_FUNCTION();
		PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		PKEngine::RenderCommand::Clear();
		PKEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	}

	{
		PK_PROFILE_FUNCTION();
		PKEngine::Renderer2D::DrawQuad(glm::vec2(0.0f,0.0f), glm::vec2(1.0f,1.0f), glm::vec4(1.0f));
		PKEngine::Renderer2D::DrawQuad(glm::vec2(-1.5,0.0f), glm::vec2(0.5f,0.5f), glm::vec4(0.8f,0.3f,0.2f,1.0f));
		//PKEngine::Renderer2D::DrawQuad(glm::vec3(1.0f,1.0f,-0.1f), glm::vec2(10.0f,10.0f), m_Texture,10);

		//PKEngine::Renderer2D::DrawRotateQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
		//PKEngine::Renderer2D::DrawRotateQuad(glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.5f), 45.0f, glm::vec4(0.8f, 0.3f, 0.2f, 1.0f));
		////for(int i=0;i<10000;i++)
		//PKEngine::Renderer2D::DrawRotateQuad(glm::vec3(1.0f, 1.0f, -0.1f), glm::vec2(10.0f, 10.0f), 60.0f, m_Texture, 10,glm::vec4(0.6f,0.3f,0.2f,0.8f));
	}
	PKEngine::Renderer2D::EndScene();

	//m_SqureShader->Bind();
//std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform4f("u_Color", m_SqureColor);

//auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
//PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader, transform);

}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqureColor));

	ImGui::End();

}

void SandBox2D::OnEvent(PKEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
