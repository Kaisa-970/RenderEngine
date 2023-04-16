#pragma once
#include "entt.hpp"
#include "PKEngine/Core/Timestep.h"
#include "PKEngine/Renderer/PerspectiveCamera.h"
#include "PKEngine/Renderer/Texture.h"
#include "PKEngine/Renderer/Shader.h"
#include "PKEngine/Renderer/Mesh.h"

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

		Ref<PerspectiveCamera> GetSceneCamera() const { return m_SceneCamera; }
	private:
		friend class Actor;

		entt::registry m_Registry;
		std::vector<Ref<Actor>> m_Actors;
		Ref<PerspectiveCamera> m_SceneCamera;

		Ref<Texture3D> m_SkyboxTexture;
		Ref<Shader> m_SkyShader;
		Ref<Mesh> m_SkyboxMesh;
	};
}