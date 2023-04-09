#include "pkpch.h"
#include "Actor.h"

namespace PKEngine
{
	Actor::Actor(entt::entity handle, Scene* scene, const std::string& name)
		:m_Handle(handle),m_Scene(scene),m_Name(name)
	{
		m_RootTransform = &AddComponent<TransformComponent>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	}

	void Actor::SetActorPosition(const glm::vec3& pos)
	{
		m_RootTransform->Position = pos;
	}

	void Actor::SetActorRotation(const glm::vec3& rot)
	{
		m_RootTransform->Rotation = rot;
	}

	void Actor::SetActorScale(const glm::vec3& sac)
	{
		m_RootTransform->Scale = sac;
	}

}