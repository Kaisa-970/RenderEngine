#include "EditorLayer.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <chrono>

namespace PKEngine {
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1920.0f / 1080.0f), m_SqureColor(1.0f)
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
	}

	void EditorLayer::OnDetach()
	{
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		PK_PROFILE_FUNCTION();

		//update
		{
			PK_PROFILE_FUNCTION();
			if(m_ViewportFocused)
				m_CameraController.Update(ts);
		}
		//rotate vector by this
		//glm::quat rrot = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::quat quat(0,glm::vec3(1.0f,0.0f,0.0f));
		//auto m = glm::vec4(1.0f,0.0f,0,0) * rrot;
		//PK_INFO("({0},{1},{2})", m.x, m.y, m.z);
		static float rotation = 0;
		rotation += ts * 20.0f;

		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		//render
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RenderCommand::Clear();
		Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_ActiveScene->OnUpdate(ts);

		Renderer2D::EndScene();

		//Renderer2D::BeginScene(m_CameraController.GetCamera());
		//for (float x = -5.0f; x < 5.0f; x += 0.5f)
		//{
		//	for (float y = -5.0f; y < 5.0f; y += 0.5f)
		//	{
		//		glm::vec4 color = { (x + 5.0f) / 10.0f,0.4f,(y + 5.0f) / 10.0f ,1.0f };
		//		Renderer2D::DrawQuad(glm::vec2(x, y), glm::vec2(0.45f), color);
		//	}
		//}
		//Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
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
		//PK_WARN("Viewport Size:{0},{1}", size.x, size.y);
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