#include "pkpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace PKEngine {
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::Init() {
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,const glm::mat4& transform)
	{
		vertexArray->Bind();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ModelMat", transform);
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		mesh->GetVertexArray()->Bind();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ModelMat", transform);
		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::SetViewProjectionMatrix(const glm::mat4& vpmat)
	{
		s_SceneData->ViewProjectionMatrix = vpmat;
	}
}