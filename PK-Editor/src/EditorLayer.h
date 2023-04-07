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
		Ref<VertexArray> m_SqureVA;
		Ref<Shader> m_SqureShader;
		glm::vec4 m_SqureColor;
		Ref<Texture2D> m_Texture;
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		// Mesh*****
		Ref<Mesh> m_Mesh;
		Ref<VertexArray> m_MeshVA;

		ShaderLibrary m_ShaderLib;
		Ref<Texture2D> m_WoodTexture;

		float m_Roughness = 0.5;
		float m_Metallic = 0;
		glm::vec3 m_LightColor = glm::vec3(1.0f);
		glm::vec3 m_LightPos = glm::vec3(1.0f, 2.0f, 0.0f);
		float m_LightIntensity = 3.0f;

		PerspectiveCamera m_PerspectiveCamera;
		glm::vec3 m_CameraPosition = glm::vec3(0, 0, -3);
		float m_CameraMoveSpeed = 1.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotateSpeed = 120.0f;
		//********

		OrthoCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Ref<Actor> m_Actor;
		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		
	};
}