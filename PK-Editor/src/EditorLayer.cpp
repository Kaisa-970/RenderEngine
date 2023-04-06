#include "EditorLayer.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <chrono>

namespace PKEngine {
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1920.0f / 1080.0f), m_SqureColor(1.0f),m_PerspectiveCamera(glm::radians(45.0f), 1.7f, 0.01f, 100.0f)

	{
	}

	void EditorLayer::OnAttach()
	{
		m_SqureVA = VertexArray::Create();

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

		Ref<VertexBuffer> squreVB;
		squreVB.reset(VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		BufferLayout squreLayout = {
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"}
		};
		squreVB->SetLayout(squreLayout);
		m_SqureVA->AddVertexBuffer(squreVB);

		Ref<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		m_SqureVA->SetIndexBuffer(squreIB);

		m_SqureShader = Shader::Create("assets/shaders/squareShader.glsl");

		m_Texture = Texture2D::Create("assets/textures/emotion1.png");

		PKEngine::FrameBufferParams fbs;
		fbs.Width = 1920;
		fbs.Height = 1080;
		m_FrameBuffer = PKEngine::FrameBuffer::Create(fbs);

		m_ActiveScene = CreateRef<Scene>();
		m_Actor = m_ActiveScene->CreateActor("Test Actor");
		m_Actor.AddComponent<SpriteComponent>();

		// test mesh*********
		m_Mesh = CreateRef<Mesh>("assets/meshes/sphere.obj");
		m_MeshVA = PKEngine::VertexArray::Create();
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

		auto MeshShader = m_ShaderLib.Load("assets/shaders/MeshShader.glsl");
		m_WoodTexture = PKEngine::Texture2D::Create("assets/textures/floor.png");
		MeshShader->Bind();
		//***********
	}

	void EditorLayer::OnDetach()
	{
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		PK_PROFILE_FUNCTION();

		//draw 2D**************
		{
			////update
			//{
			//	PK_PROFILE_FUNCTION();
			//	if (m_ViewportFocused)
			//		m_CameraController.Update(ts);
			//}

			//static float rotation = 0;
			//rotation += ts * 20.0f;

			//Renderer2D::ResetStats();

			//m_FrameBuffer->Bind();
			////render
			//RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			//RenderCommand::Clear();
			//Renderer2D::BeginScene(m_CameraController.GetCamera());
			////m_ActiveScene->OnUpdate(ts);
			//Renderer2D::EndScene();
			
			//m_FrameBuffer->Unbind();
		}
		//**************

		//draw mesh
		{
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

			float r = glm::distance(m_CameraPosition, glm::vec3(0, m_CameraPosition.y, 0));
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

			m_PerspectiveCamera.SetPosition(m_CameraPosition);

			//render
			m_FrameBuffer->Bind();

			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.3f, 0.1f, 1.0f));
			RenderCommand::Clear();
			Renderer::BeginScene(m_PerspectiveCamera);
			//m_SqureShader->Bind();
			//std::dynamic_pointer_cast<PKEngine::OpenGLShader>(m_SqureShader)->SetUniform3f("u_Color", m_SqureColor);

			auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

			auto meshShader = m_ShaderLib.Get("MeshShader");
			meshShader->Bind();
			std::dynamic_pointer_cast<OpenGLShader>(meshShader)->
				SetUniform3f("u_LightPos", m_LightPos);
			std::dynamic_pointer_cast<OpenGLShader>(meshShader)->
				SetUniform3f("u_LightColor", m_LightColor * m_LightIntensity);
			std::dynamic_pointer_cast<OpenGLShader>(meshShader)->
				SetUniformf("u_Roughness", m_Roughness);
			std::dynamic_pointer_cast<OpenGLShader>(meshShader)->
				SetUniformf("u_Metallic", m_Metallic);
			std::dynamic_pointer_cast<OpenGLShader>(meshShader)->SetUniform3f("u_CameraPos", m_PerspectiveCamera.GetPosition());

			m_SqureShader->Bind();
			Renderer::Submit(m_MeshVA, meshShader, transform);

			Renderer::EndScene();

			m_FrameBuffer->Unbind();
		}

	}

	void EditorLayer::OnImGuiRender()
	{
		/*ImGui::Begin("Settings");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw Calls: %d",stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetVertexCount());
		ImGui::Text("Index Calls: %d", stats.GetIndexCount());
		
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqureColor));
		
		ImGui::End();	*/

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static bool p_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				{
					PKEngine::Application::Get().Close();
					p_open = false;

				}
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}

		// setting panel
		ImGui::Begin("Settings");
		auto stats = PKEngine::Renderer2D::GetStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetVertexCount());
		ImGui::Text("Index Calls: %d", stats.GetIndexCount());
		//ImGui::Text("CameraPos: (%f,%f,%f)", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		
		if (m_Actor) {
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqureColor));
			m_Actor.GetComponent<SpriteComponent>().Color = m_SqureColor;
		}
		
		ImGui::End();

		// viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvent(m_ViewportFocused && m_ViewportHovered);
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&size)
		{
			m_ViewportSize.x = size.x;
			m_ViewportSize.y = size.y;

			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentID(), { m_ViewportSize.x, m_ViewportSize.y }, { 0,1 }, { 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}