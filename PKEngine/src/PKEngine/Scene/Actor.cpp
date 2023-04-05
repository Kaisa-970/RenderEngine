#include "pkpch.h"
#include "Actor.h"

namespace PKEngine
{
	Actor::Actor(entt::entity handle, Scene* scene, const std::string& name)
		:m_Handle(handle),m_Scene(scene),m_Name(name)
	{
	}
}