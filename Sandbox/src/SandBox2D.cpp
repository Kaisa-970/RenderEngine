#include "SandBox2D.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

SandBox2D::SandBox2D()
	:Layer("SandBox2D"),m_CameraController(1280.0f / 720.0f), m_SqureColor(1.0f)
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
}

void SandBox2D::OnDetach()
{
}

void SandBox2D::OnUpdate(PKEngine::Timestep ts)
{
	//update
	m_CameraController.Update(ts);


	//render
	PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	PKEngine::RenderCommand::Clear();
	PKEngine::Renderer::BeginScene(m_CameraController.GetCamera());

	m_SqureShader->Bind();
	std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform4f("u_Color", m_SqureColor);

	auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader, transform);

	PKEngine::Renderer::EndScene();
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
