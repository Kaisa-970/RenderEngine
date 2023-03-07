#include <PKEngine.h>
#include <imgui.h>

class ExampleLayer : public PKEngine::Layer {
public:
	ExampleLayer() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) , m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(PKEngine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
			 0.0f,  0.5f, 0.0f,	1.0f,1.0f,0.0f,1.0f
		};

		uint32_t indices[3] = {
			0,1,2
		};

		std::shared_ptr<PKEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(PKEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		PKEngine::BufferLayout layout = {
				{PKEngine::ShaderDataType::Float3,"a_Position"},
				{PKEngine::ShaderDataType::Float4,"a_Color",true}
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		std::shared_ptr<PKEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(PKEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);



		m_SqureVA.reset(PKEngine::VertexArray::Create());

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

		std::shared_ptr<PKEngine::VertexBuffer> squreVB;
		squreVB.reset(PKEngine::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		PKEngine::BufferLayout squreLayout = {
		{PKEngine::ShaderDataType::Float3,"a_Position"}
		};
		squreVB->SetLayout(squreLayout);
		m_SqureVA->AddVertexBuffer(squreVB);

		std::shared_ptr<PKEngine::IndexBuffer> squreIB;
		squreIB.reset(PKEngine::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		m_SqureVA->SetIndexBuffer(squreIB);

		std::string vertexS = R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMat;
			out vec4 v_Color;
			void main()
			{
			gl_Position =  u_ViewProjectionMat * position;
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

		m_Shader.reset(new PKEngine::Shader(vertexS, fragS));
		m_SqureShader.reset(new PKEngine::Shader(vertexS2, fragS2));
	}
	~ExampleLayer() {}
	virtual void OnImGuiRender()override {
		
	}

	virtual void OnUpdate(PKEngine::Timestep ts) override {
		PK_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMillionSeconds());
		float deltaTime = ts;
		if (PKEngine::Input::IsKeyPressed(PK_KEY_W)) {
			m_CameraPosition.y += deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_S)) {
			m_CameraPosition.y -= deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsKeyPressed(PK_KEY_A)) {
			m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_D)) {
			m_CameraPosition.x += deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsMouseButtonPressed(0)) {
			m_CameraRotation -= deltaTime * m_CameraRotateSpeed;
		}
		else if (PKEngine::Input::IsMouseButtonPressed(1)) {
			m_CameraRotation += deltaTime * m_CameraRotateSpeed;
		}

		PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		PKEngine::RenderCommand::Clear();
		PKEngine::Renderer::BeginScene(m_Camera);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		//m_SqureShader->Bind();
		PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader);

		//m_Shader->Bind();
		//m_Shader->SetUniformMat4f("u_ViewProjectionMat",m_Camera.GetViewProjectionMatrix());
		PKEngine::Renderer::Submit(m_VertexArray, m_Shader);

		PKEngine::Renderer::EndScene();
	}

	virtual void OnEvent(PKEngine::Event& e) override {
		//PKEngine::EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<PKEngine::KeyPressedEvent>(PK_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(PKEngine::KeyPressedEvent& e) {
		if (e.GetKeyCode() == PK_KEY_W) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_S) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_A) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_D) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		return false;
	}

private:
	std::shared_ptr<PKEngine::VertexArray> m_VertexArray;
	std::shared_ptr<PKEngine::Shader> m_Shader;


	std::shared_ptr<PKEngine::VertexArray> m_SqureVA;
	std::shared_ptr<PKEngine::Shader> m_SqureShader;

	PKEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 120.0f;
};

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		//PushOverlay(new PKEngine::ImGuiLayer());
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
