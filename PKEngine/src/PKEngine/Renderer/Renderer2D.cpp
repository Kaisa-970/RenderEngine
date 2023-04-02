#include "pkpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/gtx/transform.hpp>

namespace PKEngine {
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 Texcoord;
		//TODO: TextureID
	};

	struct Renderer2DStorage
	{
		const uint32_t MaxQuats = 10000;
		const uint32_t MaxVertices = MaxQuats*4;
		const uint32_t MaxIndices = MaxQuats*6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount;

		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;
	};



	static Renderer2DStorage s_RenderData;

	void Renderer2D::Init()
	{

		s_RenderData.QuadVertexArray = PKEngine::VertexArray::Create();

		s_RenderData.QuadVertexBufferBase = new QuadVertex[s_RenderData.MaxVertices];

		s_RenderData.QuadVertexBuffer.reset(PKEngine::VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex)));
		PKEngine::BufferLayout squreLayout = {
		{PKEngine::ShaderDataType::Float3,"a_Position"},
		{PKEngine::ShaderDataType::Float4,"a_Color" },
		{PKEngine::ShaderDataType::Float2,"a_TexCoord"}
		};
		s_RenderData.QuadVertexBuffer->SetLayout(squreLayout);
		s_RenderData.QuadVertexArray->AddVertexBuffer(s_RenderData.QuadVertexBuffer);

		uint32_t* quadIB = new uint32_t[s_RenderData.MaxIndices];
		uint32_t offset = 0;
		for (int i = 0; i < s_RenderData.MaxIndices; i += 6)
		{
			quadIB[i + 0] = offset + 0;
			quadIB[i + 1] = offset + 1;
			quadIB[i + 2] = offset + 2;

			quadIB[i + 3] = offset + 2;
			quadIB[i + 4] = offset + 3;
			quadIB[i + 5] = offset + 0;

			offset += 4;
		}
		PKEngine::Ref<PKEngine::IndexBuffer> squreIB;
		squreIB.reset(PKEngine::IndexBuffer::Create(quadIB, s_RenderData.MaxIndices));
		delete[] quadIB;
		s_RenderData.QuadVertexArray->SetIndexBuffer(squreIB);

		s_RenderData.WhiteTexture = PKEngine::Texture2D::Create(1, 1);
		uint32_t textureData = 0xffffffff;
		s_RenderData.WhiteTexture->SetData(&textureData, 4);

		s_RenderData.TextureShader = PKEngine::Shader::Create("assets/shaders/TextureShader.glsl");
		s_RenderData.TextureShader->Bind();
		s_RenderData.TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::ShutDown()
	{

	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{	
		
		s_RenderData.TextureShader->Bind();
		s_RenderData.TextureShader->SetMat4("u_ViewProjectionMat", camera.GetViewProjectionMatrix());

		s_RenderData.QuadVertexArray->Bind();

		s_RenderData.QuadIndexCount = 0;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase;
		s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray,s_RenderData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_RenderData.QuadVertexBufferPtr->Position = position;
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = {0.0f,0.0f};
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = position + glm::vec3(size.x,0,0);
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = position + glm::vec3(size.x, size.y, 0);
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = position + glm::vec3(0, size.y, 0);
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadIndexCount += 6;

		//s_RenderData.Va->Bind();

		//s_RenderData.WhiteTexture->Bind();

		//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size,1.0f));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		//glm::mat4 transform = translate * scale;
		//
		//s_RenderData.TextureShader->SetMat4("u_ModelMat", transform);
		//s_RenderData.TextureShader->SetFloat("u_Tilling", 1.0f);
		//s_RenderData.TextureShader->SetFloat4("u_Color", color);

		//RenderCommand::DrawIndexed(s_RenderData.Va);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, tilling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		s_RenderData.QuadVertexArray->Bind();
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * scale;

		texture->Bind();
		s_RenderData.TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData.TextureShader->SetFloat("u_Tilling", tilling);
		s_RenderData.TextureShader->SetMat4("u_ModelMat", transform);
	
		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_RenderData.QuadVertexArray->Bind();

		s_RenderData.WhiteTexture->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * rotate * scale;

		s_RenderData.TextureShader->SetMat4("u_ModelMat", transform);
		s_RenderData.TextureShader->SetFloat("u_Tilling", 1.0f);
		s_RenderData.TextureShader->SetFloat4("u_Color", color);

		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, texture, tilling, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		s_RenderData.QuadVertexArray->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 transform = translate * rotate * scale;

		texture->Bind();
		s_RenderData.TextureShader->SetFloat4("u_Color", tintColor);
		s_RenderData.TextureShader->SetFloat("u_Tilling", tilling);
		s_RenderData.TextureShader->SetMat4("u_ModelMat", transform);

		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray);
	}
}