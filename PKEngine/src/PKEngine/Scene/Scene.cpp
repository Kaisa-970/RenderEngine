#include "pkpch.h"
#include "Scene.h"
#include "PKEngine/Renderer/Renderer2D.h"
#include "PKEngine/Renderer/Renderer.h"
#include "Components.h"
#include "Actor.h"
#include "MeshComponent.h"
#include "LightComponent.h"

namespace PKEngine {
	Scene::Scene()
	{

		entt::entity entity = m_Registry.create();
		m_SceneCamera = CreateRef<PerspectiveCamera>(glm::radians(45.0f), 1.778f, 0.01f, 1000.0f);

		std::vector<std::string> skyboxPath =
		{
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
		};
		m_SkyboxTexture = Texture3D::Create(skyboxPath);
		m_SkyboxTexture->Bind();
		m_SkyShader = Shader::Create("assets/shaders/SkyboxShader.glsl");
		m_SkyShader->Bind();
		m_SkyShader->SetInt("u_Texture", 0);

		auto m_SkyVA = VertexArray::Create();

		float skyVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
		};

		uint32_t skyIndices[36];
		for (int i = 0; i < 36; i++)
		{
			skyIndices[i] = i;
		}

		Ref<VertexBuffer> skyVB;
		skyVB.reset(VertexBuffer::Create(skyVertices, sizeof(skyVertices)));
		BufferLayout skyLayout = {
		{ShaderDataType::Float3,"a_Position"},
		};
		skyVB->SetLayout(skyLayout);
		m_SkyVA->AddVertexBuffer(skyVB);

		Ref<IndexBuffer> skyIB;
		skyIB.reset(IndexBuffer::Create(skyIndices, sizeof(skyIndices) / sizeof(uint32_t)));
		m_SkyVA->SetIndexBuffer(skyIB);
		m_SkyboxMesh = CreateRef<Mesh>(m_SkyVA);
	}

	Scene::~Scene()
	{

	}
	void Scene::OnUpdate(Timestep ts)
	{
		{
			RenderCommand::DepthWrite(false);
			auto vp = m_SceneCamera->GetProjectionMatrix() * glm::mat4(glm::mat3(m_SceneCamera->GetViewMatrix()));
			Renderer::SetViewProjectionMatrix(vp);

			Renderer::Submit(m_SkyboxMesh, m_SkyShader);

			RenderCommand::DepthWrite(true);
			Renderer::BeginScene(*m_SceneCamera);
		}

		
		Ref<Actor> lightActor;
		for (auto actor : m_Actors)
		{
			if (actor->HasComponent<LightComponent>())
			{
				// Handle only one light
				// TODO::Handle multiple lights
				lightActor = actor;
				break;
			}
		}

		for (auto actor : m_Actors)
		{
			if (actor->HasComponent<MeshComponent>())
			{
				auto& Mesh = actor->GetComponent<MeshComponent>();
				auto& transf = actor->GetComponent<TransformComponent>();
				auto shader = Mesh.GetMaterial();
				shader->Bind();
				auto sp = Mesh.GetShaderParameters();
				if (lightActor)
				{
					auto& light = lightActor->GetComponent<LightComponent>();
					shader->SetFloat3("u_LightPos", lightActor->GetActorPosition());
					auto pos = lightActor->GetActorPosition();
					shader->SetFloat3("u_LightColor", light.GetColor()*light.GetIntensity());
					shader->SetFloat("u_Roughness", sp.Roughness);
					shader->SetFloat("u_Metallic", sp.Metallic);
					shader->SetFloat3("u_CameraPos", m_SceneCamera->GetPosition());
				}
				Renderer::Submit(Mesh.GetMesh(), shader, transf.GetMatrix());
			}
		}
	}
	Ref<Actor> Scene::CreateActor(const std::string& name)
	{
		Ref<Actor> actor = CreateRef<Actor>(m_Registry.create(), this, name);
		//actor->AddComponent<TransformComponent>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		m_Actors.push_back(actor);
		return actor;
	}
}