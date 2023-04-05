#include "pkpch.h"
#include "Scene.h"
#include "PKEngine/Renderer/Renderer2D.h"
#include "Components.h"
#include "Actor.h"

namespace PKEngine {
	Scene::Scene()
	{

		entt::entity entity = m_Registry.create();
		//m_Registry.emplace<TransformComponent>(entity, glm::vec3(0.5f, 0.0f, 0.0f));
		//m_Registry.emplace<SpriteComponent>(entity, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	Scene::~Scene()
	{

	}
	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for(auto enti:group)
		{
			auto& [tran, sprit] = group.get<TransformComponent, SpriteComponent>(enti);

			Renderer2D::DrawQuad(tran.GetMatrix(), sprit.Color);
		}
		//auto trans = view.get<TransformComponent>();
		//auto group = m_Registry.group<TransformComponent,SpriteComponent>(en)
		
	}
	Actor Scene::CreateActor(const std::string& name)
	{
		Actor actor(m_Registry.create(), this, name);
		actor.AddComponent<TransformComponent>();
		return actor;
	}
}