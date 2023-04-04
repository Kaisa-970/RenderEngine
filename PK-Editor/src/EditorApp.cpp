#include <PKEngine.h>
#include <PKEngine/Core/EntryPoint.h>
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "EditorLayer.h"

namespace PKEngine {
	class ExampleLayer : public PKEngine::Layer {
	public:
		ExampleLayer() : m_CameraController(1280.0f / 720.0f), m_SqureColor(0.0f)
		{
			//m_ShaderLibrary = new PKEngine::ShaderLibrary();
			m_VertexArray = PKEngine::VertexArray::Create();

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
				 0.0f,  0.5f, 0.0f,	1.0f,1.0f,0.0f,1.0f
			};

			uint32_t indices[3] = {
				0,1,2
			};

			PKEngine::Ref<PKEngine::VertexBuffer> m_VertexBuffer;
			m_VertexBuffer.reset(PKEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

			PKEngine::BufferLayout layout = {
					{PKEngine::ShaderDataType::Float3,"a_Position"},
					{PKEngine::ShaderDataType::Float4,"a_Color",true}
			};
			m_VertexBuffer->SetLayout(layout);

			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			PKEngine::Ref<PKEngine::IndexBuffer> m_IndexBuffer;
			m_IndexBuffer.reset(PKEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_VertexArray->SetIndexBuffer(m_IndexBuffer);



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

			std::string vertexS = R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMat;
			uniform mat4 u_ModelMat;
			out vec4 v_Color;
			void main()
			{
			gl_Position =  u_ViewProjectionMat * u_ModelMat * position;
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
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ModelMat;
			uniform mat4 u_ViewProjectionMat;
			out vec2 o_TexCoord;
			void main()
			{
			gl_Position = u_ViewProjectionMat * u_ModelMat * a_position;
			o_TexCoord = a_TexCoord;
			}; )";

			std::string fragS2 = R"(#version 330 core
			out vec4 color;
			uniform vec3 u_Color;
			in vec2 o_TexCoord;
			void main()
			{
			color = vec4(u_Color,1.0f);
			};)";

			m_Shader = PKEngine::Shader::Create("triangleShader", vertexS, fragS);
			m_SqureShader = PKEngine::Shader::Create("squareShader", vertexS2, fragS2);

			auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");

			m_Texture = PKEngine::Texture2D::Create("assets/textures/emotion1.png");

			textureShader->Bind();
			m_Texture->Bind();
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(textureShader)->SetUniformi("u_Texture", 0);
		}
		~ExampleLayer() {}
		virtual void OnImGuiRender()override {
			ImGui::Begin("Settings");
			ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SqureColor));
			ImGui::End();
		}

		virtual void OnUpdate(PKEngine::Timestep ts) override {
			//PK_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMillionSeconds());
			float deltaTime = ts;

			//update
			m_CameraController.Update(ts);


			//render
			PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			PKEngine::RenderCommand::Clear();
			PKEngine::Renderer::BeginScene(m_CameraController.GetCamera());

			m_SqureShader->Bind();
			std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform3f("u_Color", m_SqureColor);

			//auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			//for (int i = 0; i < 20; i++) {
			//	for (int j = 0; j < 20; j++)
			//	{
			//		glm::vec3 pos(i * 0.11f, j * 0.11f, 0);
			//		auto transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			//		PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader, transform);
			//	}
			//	
			//}

			auto textureShader = m_ShaderLibrary.Get("TextureShader");
			auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
			PKEngine::Renderer::Submit(m_SqureVA, textureShader, transform);

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
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;


		Ref<VertexArray> m_SqureVA;
		Ref<Shader> m_SqureShader;
		glm::vec4 m_SqureColor;

		ShaderLibrary m_ShaderLibrary;

		OrthoCameraController m_CameraController;

		Ref<Texture2D> m_Texture;
	};

	class PKEditor : public PKEngine::Application {
	public:
		PKEditor():Application("PKEditor")
		{
			//PushLayer(new ExampleLayer());

			PushLayer(new EditorLayer());

		}

		~PKEditor() {

		}
	};

	PKEngine::Application* PKEngine::CreateApplication() {

		return new PKEditor();
	}
}