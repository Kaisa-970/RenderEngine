#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtx/transform.hpp>

namespace PKEngine {
	struct Renderer2DStorage
	{
		Ref<VertexArray> Va;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_RenderData;

	void Renderer2D::Init()
	{
		s_RenderData = new Renderer2DStorage();

		s_RenderData->Va = PKEngine::VertexArray::Create();

		float squreVertices[] = {
	-0.5f, -0.5f, 0.0f, 0.0f,0.0f,
	 0.5f, -0.5f, 0.0f,	1.0f,0.0f,
	 0.5f,  0.5f, 0.0f,	1.0f,1.0f,
	 -0.5f, 0.5f, 0.0f,	0.0f,1.0f
		};

		uint32_t squreIndices[6] = {
			0,1,2,
			2,3,0
		};

		PKEngine::Ref<PKEngine::VertexBuffer> squreVB;
		squreVB.reset(PKEngine::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		PKEngine::BufferLayout squreLayout = {
		{PKEngine::ShaderDataType::Float3,"a_Position"},
		{PKEngine::ShaderDataType::Float2,"a_TexCoord"}
		};
		squreVB->SetLayout(squreLayout);
		s_RenderData->Va->AddVertexBuffer(squreVB);

		PKEngine::Ref<PKEngine::IndexBuffer> squreIB;
		squreIB.reset(PKEngine::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		s_RenderData->Va->SetIndexBuffer(squreIB);

		s_RenderData->WhiteTexture = PKEngine::Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_RenderData->WhiteTexture->SetData(&textureData, 4);

		s_RenderData->TextureShader = PKEngine::Shader::Create("assets/shaders/TextureShader.glsl");
		s_RenderData->TextureShader->Bind();
		s_RenderData->TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::ShutDown()
	{
		delete s_RenderData;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{	
		
		s_RenderData->TextureShader->Bind();
		s_RenderData->TextureShader->SetMat4("u_ViewProjectionMat", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_RenderData->Va->Bind();

		s_RenderData->WhiteTexture->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size,1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * scale;
		
		s_RenderData->TextureShader->SetMat4("u_ModelMat", transform);
		s_RenderData->TextureShader->SetFloat("u_Tilling", 1.0f);
		s_RenderData->TextureShader->SetFloat4("u_Color", color);

		RenderCommand::DrawIndexed(s_RenderData->Va);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, tilling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		s_RenderData->Va->Bind();
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * scale;

		texture->Bind();
		s_RenderData->TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData->TextureShader->SetFloat("u_Tilling", tilling);
		s_RenderData->TextureShader->SetMat4("u_ModelMat", transform);
	
		RenderCommand::DrawIndexed(s_RenderData->Va);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_RenderData->Va->Bind();

		s_RenderData->WhiteTexture->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * rotate * scale;

		s_RenderData->TextureShader->SetMat4("u_ModelMat", transform);
		s_RenderData->TextureShader->SetFloat("u_Tilling", 1.0f);
		s_RenderData->TextureShader->SetFloat4("u_Color", color);

		RenderCommand::DrawIndexed(s_RenderData->Va);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, texture, tilling, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		s_RenderData->Va->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * rotate * scale;

		texture->Bind();
		s_RenderData->TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData->TextureShader->SetFloat("u_Tilling", tilling);
		s_RenderData->TextureShader->SetMat4("u_ModelMat", transform);

		RenderCommand::DrawIndexed(s_RenderData->Va);
	}
}