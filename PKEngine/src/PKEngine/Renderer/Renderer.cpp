#include "pkpch.h"
#include "Renderer.h"

namespace PKEngine {
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		vertexArray->Bind();

		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		RenderCommand::DrawIndexed(vertexArray);
	}
}