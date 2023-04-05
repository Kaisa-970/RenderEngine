#pragma once
#include "entt.hpp"
#include "PKEngine/Core/Timestep.h"

namespace PKEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		const entt::registry& GetReg() { return m_Registry; }
	private:
		entt::registry m_Registry;
	};
}