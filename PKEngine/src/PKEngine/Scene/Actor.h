#pragma once
#include "Scene.h"
#include "glm/glm.hpp"
#include "PKEngine/Scene/Components.h"

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
			return m_Scene->m_Registry.all_of<T>(m_Handle);
			//return m_Scene->m_Registry.has<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PK_CORE_ASSERT(HasComponent<T>(), "Can not remove component which is not exist!");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		std::string GetName() { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		operator bool() const { return (uint32_t)m_Handle != entt::null; }

		bool operator==(const Actor* other) { 
			if (other == nullptr) { return false; } 
			return m_Handle == other->m_Handle && m_Scene == other->m_Scene; 
		}
		bool operator!=(const Actor* other) { return !operator==(other); }
	
	public:
		void SetActorPosition(const glm::vec3& pos);
		void SetActorRotation(const glm::vec3& rot);
		void SetActorScale(const glm::vec3& sac);

		glm::vec3 GetActorPosition()const{return m_RootTransform->Position;};
		glm::vec3 GetActorRotation()const{return m_RootTransform->Rotation;};
		glm::vec3 GetActorScale()const   {return m_RootTransform->Scale;};

	private:
		entt::entity m_Handle{entt::null};
		Scene* m_Scene = nullptr;
		std::string m_Name;
		TransformComponent* m_RootTransform;
	};
}