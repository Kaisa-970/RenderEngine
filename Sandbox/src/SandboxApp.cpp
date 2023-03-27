#include <PKEngine.h>
#include <PKEngine/Core/EntryPoint.h>
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "SandBox2D.h"


class ExampleLayer : public PKEngine::Layer {
public:
	ExampleLayer() : m_PerspectiveCamera(glm::radians(45.0f), 1.7f, 0.01f, 100.0f) , m_CameraPosition(0.0f), m_SqureColor(0.0f)
	{
		//*********Floor
		m_FloorVA.reset(PKEngine::VertexArray::Create());

		float vertices[] = {
			-10.0f, -1.0f, -10.0f, 0.0f,0.0f,0.0f,0.0f,1.0f,
			-10.0f, -1.0f,  10.0f, 1.0f,0.0f,0.0f,0.0f,1.0f,
			 10.0f, -1.0f,  10.0f, 1.0f,1.0f,0.0f,0.0f,1.0f,
			 10.0f, -1.0f, -10.0f, 0.0f,1.0f,0.0f,0.0f,1.0f
		};

		uint32_t indices[] = {
			0,1,2,
			2,3,0
		};

		PKEngine::Ref<PKEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(PKEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		PKEngine::BufferLayout layout = {
				{PKEngine::ShaderDataType::Float3,"a_Position"},
				{PKEngine::ShaderDataType::Float2,"a_Texcoord"},
				{PKEngine::ShaderDataType::Float3,"a_Normal"}
		};
		m_VertexBuffer->SetLayout(layout);

		m_FloorVA->AddVertexBuffer(m_VertexBuffer);

		PKEngine::Ref<PKEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(PKEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_FloorVA->SetIndexBuffer(m_IndexBuffer);
		//*********

		//*************Mesh
		m_Mesh = std::make_shared<PKEngine::Mesh>("assets/meshes/sphere.obj");
		m_MeshVA.reset(PKEngine::VertexArray::Create());
		PKEngine::Ref<PKEngine::VertexBuffer> m_MeshBuffer;
		m_MeshBuffer.reset(PKEngine::VertexBuffer::Create(m_Mesh->GetVertices(), m_Mesh->GetVerticesCount() * 8 * sizeof(float)));

		PKEngine::BufferLayout meshlayout = {
				{PKEngine::ShaderDataType::Float3,"a_Position"},
				{PKEngine::ShaderDataType::Float2,"a_Texcoord"},
				{PKEngine::ShaderDataType::Float3,"a_Normal"}
		};
		m_MeshBuffer->SetLayout(meshlayout);

		m_MeshVA->AddVertexBuffer(m_MeshBuffer);

		PKEngine::Ref<PKEngine::IndexBuffer> m_MeshIndexBuffer;
		PK_INFO("Mesh indices size:{0}", m_Mesh->GetFacesCount());
		m_MeshIndexBuffer.reset(PKEngine::IndexBuffer::Create(m_Mesh->GetIndices(), m_Mesh->GetFacesCount() * 3));
		m_MeshVA->SetIndexBuffer(m_MeshIndexBuffer);
		//************
		auto p = glm::perspective(glm::radians(45.0f), 1.7f, 0.01f, 10.0f);
		


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

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
		auto DefaultShader = m_ShaderLibrary.Load("assets/shaders/DefaultShader.glsl");
		auto FloorShader = m_ShaderLibrary.Load("assets/shaders/FloorShader.glsl");
		auto MeshShader = m_ShaderLibrary.Load("assets/shaders/MeshShader.glsl");
		
		m_Texture = PKEngine::Texture2D::Create("assets/textures/emotion1.png");
		m_WoodTexture = PKEngine::Texture2D::Create("assets/textures/floor.png");

		textureShader->Bind();
		m_Texture->Bind();
		std::dynamic_pointer_cast<PKEngine::OpenGLShader>(textureShader)->SetUniformi("u_Texture", 0);

		MeshShader->Bind();
		//std::dynamic_pointer_cast<PKEngine::OpenGLShader>(MeshShader)->SetUniformi("u_Texture", 0);

		FloorShader->Bind();
		m_WoodTexture->Bind();
		std::dynamic_pointer_cast<PKEngine::OpenGLShader>(FloorShader)->SetUniformi("u_Texture", 0);

		m_CameraPosition = glm::vec3(0, 0, -3);
	}
	~ExampleLayer() {}
	virtual void OnImGuiRender()override {
		ImGui::Begin("Settings");
		float minData = 0;
		float maxData = 1;
		float lminData = 0;
		float lmaxData = 10;
		ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
		ImGui::SliderScalar("Light Intensity", ImGuiDataType_Float, &m_LightIntensity, &lminData, &lmaxData);
		ImGui::SliderScalar("Roughness",ImGuiDataType_Float, &m_Roughness, &minData, &maxData);
		ImGui::SliderScalar("Metallic", ImGuiDataType_Float, &m_Metallic, &minData, &maxData);
		ImGui::End();
	}

	virtual void OnUpdate(PKEngine::Timestep ts) override {
		//PK_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMillionSeconds());
		float deltaTime = ts;
		glm::vec3 forward = -glm::normalize(m_PerspectiveCamera.GetPosition());
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		if (PKEngine::Input::IsKeyPressed(PK_KEY_W)) {
			//m_CameraPosition.y += deltaTime * m_CameraMoveSpeed;
			m_CameraPosition += forward * deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_S)) {
			//m_CameraPosition.y -= deltaTime * m_CameraMoveSpeed;
			m_CameraPosition -= forward * deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsKeyPressed(PK_KEY_A)) {
			//m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
			m_CameraPosition -= right * deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_D)) {
			//m_CameraPosition.x += deltaTime * m_CameraMoveSpeed;
			m_CameraPosition += right * deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsKeyPressed(PK_KEY_E)) {
			//m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
			m_CameraPosition.y += deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_Q)) {
			//m_CameraPosition.x += deltaTime * m_CameraMoveSpeed;
			m_CameraPosition.y -= deltaTime * m_CameraMoveSpeed;
		}

		float r = glm::distance(m_CameraPosition,glm::vec3(0, m_CameraPosition.y,0));
		if (PKEngine::Input::IsMouseButtonPressed(0)) {
			m_CameraRotation -= deltaTime * m_CameraRotateSpeed;

			m_CameraPosition.x = glm::cos(glm::radians(m_CameraRotation)) * r;
			m_CameraPosition.z = glm::sin(glm::radians(m_CameraRotation)) * r;
		}
		else if (PKEngine::Input::IsMouseButtonPressed(1)) {
			m_CameraRotation += deltaTime * m_CameraRotateSpeed;

			m_CameraPosition.x = glm::cos(glm::radians(m_CameraRotation)) * r;
			m_CameraPosition.z = glm::sin(glm::radians(m_CameraRotation)) * r;
		}

		//render
		PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		PKEngine::RenderCommand::Clear();
		PKEngine::Renderer::BeginScene(m_PerspectiveCamera);

		//m_SqureShader->Bind();
		//std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform3f("u_Color", m_SqureColor);
		m_PerspectiveCamera.SetPosition(m_CameraPosition);
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//m_Camera.SetRotation(m_CameraRotation);
		auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		//draw floor
		{
			auto floorShader = m_ShaderLibrary.Get("FloorShader");
			floorShader->Bind();
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(floorShader)->
				SetUniform3f("u_LightPos", m_LightPos);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(floorShader)->
				SetUniform3f("u_LightColor", m_LightColor * m_LightIntensity);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(floorShader)->SetUniform3f("u_CameraPos", m_CameraPosition);
			//auto meshShader = m_ShaderLibrary.Get("MeshShader");
			PKEngine::Renderer::Submit(m_FloorVA, floorShader, transform);
		}

		//draw mesh
		{
			auto meshShader = m_ShaderLibrary.Get("MeshShader");
			meshShader->Bind();
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(meshShader)->
				SetUniform3f("u_LightPos", m_LightPos);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(meshShader)->
				SetUniform3f("u_LightColor", m_LightColor * m_LightIntensity);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(meshShader)->
				SetUniformf("u_Roughness", m_Roughness);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(meshShader)->
				SetUniformf("u_Metallic", m_Metallic);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(meshShader)->SetUniform3f("u_CameraPos", m_CameraPosition);
			//auto meshShader = m_ShaderLibrary.Get("MeshShader");
			PKEngine::Renderer::Submit(m_MeshVA, meshShader, transform);
		}

		//draw lightshap
		{
			auto defaultShader = m_ShaderLibrary.Get("DefaultShader");
			defaultShader->Bind();
			transform = glm::translate(glm::mat4(1.0f), m_LightPos);
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(defaultShader)->
				SetUniform3f("u_LightColor", m_LightColor);
			transform = glm::scale(transform, glm::vec3(0.2f));
			PKEngine::Renderer::Submit(m_MeshVA, defaultShader, transform);
		}

		PKEngine::Renderer::EndScene();
	}

	virtual void OnEvent(PKEngine::Event& e) override {
		//PKEngine::EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<PKEngine::KeyPressedEvent>(PK_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
		m_CameraController.OnEvent(e);
	}

	bool OnKeyPressed(PKEngine::KeyPressedEvent& e) {

		return false;
	}

private:
	//PKEngine::Ref<PKEngine::VertexArray> m_VertexArray;
	//PKEngine::Ref<PKEngine::Shader> m_Shader;


	PKEngine::Ref<PKEngine::VertexArray> m_SqureVA;
	PKEngine::Ref<PKEngine::Shader> m_SqureShader;
	glm::vec3 m_SqureColor;

	PKEngine::Ref<PKEngine::VertexArray> m_FloorVA;
	PKEngine::Ref<PKEngine::Shader> m_FloorShader;

	PKEngine::Ref<PKEngine::VertexArray> m_MeshVA;
	PKEngine::Ref<PKEngine::Shader> m_MeshShader;

	PKEngine::ShaderLibrary m_ShaderLibrary;

	//PKEngine::OrthographicCamera m_OthorCamera;
	glm::vec3 m_CameraPosition = glm::vec3(0,0,-3);
	float m_CameraMoveSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 120.0f;

	PKEngine::PerspectiveCamera m_PerspectiveCamera;

	PKEngine::Ref<PKEngine::Texture2D> m_Texture;
	PKEngine::Ref<PKEngine::Texture2D> m_WoodTexture;
	PKEngine::Ref<PKEngine::Mesh> m_Mesh;

	float m_Roughness = 0.5;
	float m_Metallic = 0;
	glm::vec3 m_LightColor = glm::vec3(1.0f);
	glm::vec3 m_LightPos = glm::vec3(1.0f,2.0f,0.0f);
	float m_LightIntensity = 3.0f;
};

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());

		PushLayer(new SandBox2D());

	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
