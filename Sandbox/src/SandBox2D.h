#pragma once
#include <PKEngine.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

class SandBox2D : public PKEngine::Layer {
public:
	SandBox2D();
	virtual ~SandBox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(PKEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(PKEngine::Event& e) override;

private:
	PKEngine::Ref<PKEngine::VertexArray> m_SqureVA;
	PKEngine::Ref<PKEngine::Shader> m_SqureShader;
	glm::vec4 m_SqureColor;

	//PKEngine::ShaderLibrary m_ShaderLibrary;

	PKEngine::OrthoCameraController m_CameraController;
};