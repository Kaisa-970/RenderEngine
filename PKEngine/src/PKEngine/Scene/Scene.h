#pragma once
#include "entt.hpp"
#include "PKEngine/Core/Timestep.h"

namespace PKEngine {
	class Actor;
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		Ref<Actor> CreateActor(const std::string& name = "Actor");

		std::vector<Ref<Actor>>& GetAllActors() { return m_Actors; }

		const entt::registry& GetReg() { return m_Registry; }
	private:
		entt::registry m_Registry;
		std::vector<Ref<Actor>> m_Actors;

		friend class Actor;
	};
}