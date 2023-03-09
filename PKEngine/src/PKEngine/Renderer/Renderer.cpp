#include "pkpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace PKEngine {
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader,const glm::mat4& transform)
	{
		vertexArray->Bind();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ModelMat", transform);
		RenderCommand::DrawIndexed(vertexArray);
	}
}