#include "EditorLayer.h"
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <chrono>
#include <PKEngine/Scene/MeshComponent.h>

namespace PKEngine {
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1920.0f / 1080.0f), m_SqureColor(1.0f),m_PerspectiveCamera(glm::radians(45.0f), 1.778f, 0.01f, 100.0f)

	{
	}

	void EditorLayer::OnAttach()
	{
		m_SqureVA = VertexArray::Create();

		float squreVertices[] = {
	-20.0f, -1.0f, -20.0f, 0.0f,0.0f, 0.0f,1.0f,0.0f,
	 20.0f, -1.0f, -20.0f, 1.0f,0.0f, 0.0f,1.0f,0.0f,
	 20.0f, -1.0f,  20.0f, 1.0f,1.0f, 0.0f,1.0f,0.0f,
	-20.0f, -1.0f,  20.0f, 0.0f,1.0f, 0.0f,1.0f,0.0f
		};

		uint32_t squreIndices[6] = {
			0,1,2,
			2,3,0
		};

		Ref<VertexBuffer> squreVB;
		squreVB.reset(VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		BufferLayout squreLayout = {
		{ShaderDataType::Float3,"a_Position"},
		{ShaderDataType::Float2,"a_TexCoord"},
		{ShaderDataType::Float3,"a_Normal"}
		};
		squreVB->SetLayout(squreLayout);
		m_SqureVA->AddVertexBuffer(squreVB);

		Ref<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		m_SqureVA->SetIndexBuffer(squreIB);

		m_SqureShader = Shader::Create("assets/shaders/squareShader.glsl");

		m_WhiteTexture = Texture2D::Create(1,1);
		uint32_t textureData = 0xffffffff;
		m_WhiteTexture->SetData(&textureData, 4);
		m_WhiteTexture->Bind(0);

		m_Texture = Texture2D::Create("assets/textures/emotion1.png");
		m_Texture->Bind(1);

		PKEngine::FrameBufferParams fbs;
		fbs.Width = 1920;
		fbs.Height = 1080;
		m_FrameBuffer = PKEngine::FrameBuffer::Create(fbs);

		m_ActiveScene = CreateRef<Scene>();
		m_Actor = m_ActiveScene->CreateActor("Houtou");
		m_Actor->AddComponent<SpriteComponent>();
		m_ActorArray.push_back(m_Actor);

		auto sphereActor = m_ActiveScene->CreateActor("Sphere");
		sphereActor->SetActorPosition(glm::vec3(2.0f, 0.5f, -2.5f));
		m_ActorArray.push_back(sphereActor);

		m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_ActiveScene);

		// test mesh*********
		m_Mesh = CreateRef<Mesh>("assets/meshes/houtou.obj");
		m_MeshArray.push_back(m_Mesh);
		auto m_SphereMesh = CreateRef<Mesh>("assets/meshes/sphere.obj");
		m_MeshArray.push_back(m_SphereMesh);
		//m_MeshVA = PKEngine::VertexArray::Create();
		//PKEngine::Ref<PKEngine::VertexBuffer> m_MeshBuffer;
		//m_MeshBuffer.reset(PKEngine::VertexBuffer::Create((float*)m_Mesh->GetVertices(), m_Mesh->GetVerticesCount()*sizeof(Vertex)));

		//PKEngine::BufferLayout meshlayout = {
		//		{PKEngine::ShaderDataType::Float3,"a_Position"},
		//		{PKEngine::ShaderDataType::Float2,"a_Texcoord"},
		//		{PKEngine::ShaderDataType::Float3,"a_Normal"}
		//};
		//m_MeshBuffer->SetLayout(meshlayout);

		//m_MeshVA->AddVertexBuffer(m_MeshBuffer);

		//PKEngine::Ref<PKEngine::IndexBuffer> m_MeshIndexBuffer;
		//m_MeshIndexBuffer.reset(PKEngine::IndexBuffer::Create((uint32_t*)m_Mesh->GetTriangles(), m_Mesh->GetTrianglesCount() * 3));
		//m_MeshVA->SetIndexBuffer(m_MeshIndexBuffer);

		auto MeshShader = m_ShaderLib.Load("assets/shaders/MeshShader.glsl");

		auto floorShader = m_ShaderLib.Load("assets/shaders/FloorShader.glsl");
		m_WoodTexture = PKEngine::Texture2D::Create("assets/textures/floor.png");
		m_WoodTexture->Bind(2);
		floorShader->Bind();
		floorShader->SetInt("u_Texture", 2);
		m_Actor->AddComponent<MeshComponent>(m_Mesh,MeshShader);
		sphereActor->AddComponent<MeshComponent>(m_SphereMesh, MeshShader);

		//***********
	}

	void EditorLayer::OnDetach()
	{
	}

	float time = 0;
	void EditorLayer::OnUpdate(Timestep ts)
	{
		PK_PROFILE_FUNCTION();

		//draw mesh
		{
			if (m_ViewportFocused) 
			{
				float deltaTime = ts;
				glm::vec3 forward = m_PerspectiveCamera.GetForward();
				glm::vec3 right = m_PerspectiveCamera.GetRight();
				m_CameraPosition = m_PerspectiveCamera.GetPosition();
				if (PKEngine::Input::IsKeyPressed(PK_KEY_W)) {
					//PK_CORE_INFO("forward:({0},{1},{2})", forward.x, forward.y, forward.z);
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

				float xPos = Input::GetMouseX();
				float yPos = Input::GetMouseY();
				float deltaX = xPos - m_LastMousePosX;
				float deltaY = yPos - m_LastMousePosY;
				m_LastMousePosX = xPos;
				m_LastMousePosY = yPos;

				m_CameraRotation = m_PerspectiveCamera.GetRotation();
				if (PKEngine::Input::IsMouseButtonPressed(0)) {
					//m_CameraRotation.y -= deltaTime * m_CameraRotateSpeed;
				}
				else if (PKEngine::Input::IsMouseButtonPressed(1)) {
					//PK_CORE_INFO("mouse pos:({0},{1})", deltaX, deltaY);
					//m_CameraRotation.y += deltaTime * m_CameraRotateSpeed;
					//auto [x, y] = Input::GetMousePosition();
					m_CameraRotation.y -= deltaX * deltaTime * m_CameraRotateSpeed * 0.02f;
					m_CameraRotation.x -= deltaY * deltaTime * m_CameraRotateSpeed * 0.02f;
				}

				if (PKEngine::Input::IsKeyPressed(PK_KEY_SPACE)) {
					//m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
					m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
					m_CameraRotation = glm::vec3(0.0f);
				}

				m_PerspectiveCamera.SetRotation(m_CameraRotation);

				m_PerspectiveCamera.SetPosition(m_CameraPosition);
			}

			time += ts.GetSeconds();
			auto rotation = m_Actor->GetActorRotation();
			rotation.y += ts.GetSeconds() * 60.0f;
			//m_Actor->SetActorRotation(rotation);
			//render
			m_FrameBuffer->Bind();

			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();
			Renderer::BeginScene(m_PerspectiveCamera);


			for (int i = 0; i < m_ActorArray.size(); i++)
			{
				auto transform = m_ActorArray[i]->GetComponent<TransformComponent>().GetMatrix();
				auto meshShader = m_ActorArray[i]->GetComponent<MeshComponent>().GetMaterial();
				meshShader->Bind();

				meshShader->SetFloat3("u_LightPos", m_LightPos);
				meshShader->SetFloat3("u_LightColor", m_LightColor * m_LightIntensity);
				meshShader->SetFloat("u_Roughness", m_Roughness);
				meshShader->SetFloat("u_Metallic", m_Metallic);
				meshShader->SetFloat3("u_CameraPos", m_PerspectiveCamera.GetPosition());
				if (i == 0)
					m_WhiteTexture->Bind();
				meshShader->SetInt("u_Texture", i);
				Renderer::Submit(m_MeshArray[i], meshShader, transform);
			}
				

			{
				auto floorShader = m_ShaderLib.Get("FloorShader");
				floorShader->Bind();
				floorShader->SetFloat3("u_LightPos", m_LightPos);
				floorShader->SetFloat3("u_LightColor", m_LightColor * m_LightIntensity);

				floorShader->SetFloat3("u_CameraPos", m_PerspectiveCamera.GetPosition());

				Renderer::Submit(m_SqureVA, floorShader, glm::mat4(1.0f));
			}
			Renderer::EndScene();

			m_FrameBuffer->Unbind();
		}

	}

	void EditorLayer::OnImGuiRender()
	{
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

		ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
		ImGui::SliderFloat("Light Intensity", &m_LightIntensity, 0, 10);
		ImGui::SliderFloat("Roughness", &m_Roughness, 0, 1);
		ImGui::SliderFloat("Metallic", &m_Metallic, 0, 1);

		ImGui::SliderFloat("Camera Fov", &m_CameraFov, 5, 75);
		m_PerspectiveCamera.SetFov(m_CameraFov);

		//ImGui::SliderFloat("Camera AspectRatio", &m_CameraAspect, 1, 3);
		//m_PerspectiveCamera.SetAspect(m_CameraAspect);

		auto& trans = m_Actor->GetComponent<TransformComponent>();
		ImGui::DragFloat3("Rotation", &trans.Rotation.x);

		//ImGui::Text("CameraPos: (%f,%f,%f)", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		
		if (m_Actor) {
			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SqureColor));
			m_Actor->GetComponent<SpriteComponent>().Color = m_SqureColor;
		}
		
		ImGui::End();

		m_SceneHierarchyPanel->OnImGuiRender();

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
			m_PerspectiveCamera.SetAspect(m_ViewportSize.x / m_ViewportSize.y);
		}
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentID(), { m_ViewportSize.x, m_ViewportSize.y }, { 0,1 }, { 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_PerspectiveCamera.OnEvent(e);
	}
}