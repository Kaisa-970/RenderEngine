#pragma once
#include <PKEngine.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "PKEngine/Scene/Scene.h"

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

		Ref<Mesh> m_Mesh;

		OrthoCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Actor m_Actor;
	};
}