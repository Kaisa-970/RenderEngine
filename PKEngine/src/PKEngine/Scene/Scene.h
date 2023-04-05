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

		Actor CreateActor(const std::string& name = "Actor");

		const entt::registry& GetReg() { return m_Registry; }
	private:
		entt::registry m_Registry;

		friend class Actor;
	};
}