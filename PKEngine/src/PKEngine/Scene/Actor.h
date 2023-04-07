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

		std::string GetName() { return m_Name; }

		operator bool() const { return (uint32_t)m_Handle != entt::null; }

		bool operator==(const Actor* other) { 
			if (other == nullptr) { return false; } 
			return m_Handle == other->m_Handle && m_Scene == other->m_Scene; 
		}
		bool operator!=(const Actor* other) { return !operator==(other); }
	private:
		entt::entity m_Handle{entt::null};
		Scene* m_Scene = nullptr;
		std::string m_Name;
	};
}