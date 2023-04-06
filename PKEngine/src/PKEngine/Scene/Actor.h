#pragma once
#include "Scene.h"

namespace PKEngine
{
	class Actor
	{
	public:
		Actor() = default;
		Actor(entt::entity handle, Scene* scene,const std::string& name);
		~Actor() = default;

		template<typename T,typename ... Args>
		T& AddComponent(Args&& ...args)
		{
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PK_CORE_ASSERT(HasComponent<T>(), "Can not remove component which is not exist!");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		operator bool() const { return (uint32_t)m_Handle != entt::null; }

	private:
		entt::entity m_Handle{entt::null};
		Scene* m_Scene = nullptr;
		std::string m_Name;
	};
}