#pragma once
#include "entt.hpp"
namespace PKEngine {

	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;
	};
}