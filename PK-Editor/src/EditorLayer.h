#pragma once
#include <PKEngine.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "PKEngine/Scene/Scene.h"
#include "Panels/SceneHierarchyPanel.h"

namespace PKEngine{
	class EditorLayer : public PKEngine::Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(PKEngine::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(PKEngine::Event& e) override;

	private:
		Ref<Texture2D> m_Texture;
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		// Mesh*****

		ShaderLibrary m_ShaderLib;
		Ref<Texture2D> m_WoodTexture;

		Ref<PerspectiveCamera> m_PerspectiveCamera;
		glm::vec3 m_CameraPosition = glm::vec3(0, 0, -3);
		float m_CameraMoveSpeed = 3.0f;
		float m_CameraFov = 45.0f;
		float m_CameraAspect = 1.778f;

		glm::vec3 m_CameraRotation = glm::vec3(0, 0, 0);
		float m_CameraRotateSpeed = 120.0f;

		float m_LastMousePosX;
		float m_LastMousePosY;

		Ref<Texture2D> m_WhiteTexture;

		Ref<Mesh> m_XiangZi;
		Ref<Texture2D> m_DiffXZ;
		Ref<Texture2D> m_NormXZ;
		//********

		Ref<Scene> m_ActiveScene;
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		std::vector<Ref<Actor>> m_ActorArray;
		Ref<Actor> m_PointLight;
	};
}