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
		float TextureID;
		float TillingFactor;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuats = 10000;
		static const uint32_t MaxVertices = MaxQuats*4;
		static const uint32_t MaxIndices = MaxQuats*6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount;

		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex;

		std::array<glm::vec4, 4> QuadVertexPositions;

		Renderer2D::Statistics Stats;
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
		{PKEngine::ShaderDataType::Float2,"a_TexCoord"},
		{PKEngine::ShaderDataType::Float,"a_TextureIndex"},
		{PKEngine::ShaderDataType::Float,"a_TillingFactor"}
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

		s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;

		s_RenderData.TextureShader = PKEngine::Shader::Create("assets/shaders/TextureShader.glsl");
		s_RenderData.TextureShader->Bind();
		int samplers[s_RenderData.MaxTextureSlots];
		for (int i = 0; i < s_RenderData.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}
		s_RenderData.TextureShader->SetIntArray("u_Texture", samplers, s_RenderData.MaxTextureSlots);

		s_RenderData.QuadVertexPositions[0] = { -0.5f,-0.5f, 0.0f,1.0f };
		s_RenderData.QuadVertexPositions[1] = {  0.5f,-0.5f, 0.0f,1.0f };
		s_RenderData.QuadVertexPositions[2] = {  0.5f, 0.5f, 0.0f,1.0f };
		s_RenderData.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f,1.0f };
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

		s_RenderData.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase;
		s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{

		for (int i = 0; i < s_RenderData.TextureSlotIndex; i++)
		{
			s_RenderData.TextureSlots[i]->Bind(i);

		}
		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray,s_RenderData.QuadIndexCount);

		s_RenderData.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_RenderData.QuadIndexCount = 0;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;

		s_RenderData.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_RenderData.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}

		float textureID = 0.0f;//white texure
		float tillingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), 0.0f,glm::vec3(0.0f,0.0f,1.0f))
			* glm::scale(glm::mat4(1.0f), {size.x,size.y,1.0f});

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[0];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = {0.0f,0.0f};
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[1];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[2];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[3];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, tilling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		if (s_RenderData.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}
		float textureID = 0.0f;
		float tillingFactor = tilling;

		glm::vec4 color(1.0f);

		for (int i = 0; i < s_RenderData.TextureSlotIndex; i++)
		{
			if (*s_RenderData.TextureSlots[i] == *texture)
			{
				textureID = i;
				break;
			}
		}

		if (textureID == 0.0f)
		{
			textureID = s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[0];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[1];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[2];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[3];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		if (s_RenderData.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}
		float textureID = 0.0f;//white texure
		float tillingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[0];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[1];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[2];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[3];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation, texture, tilling, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilling, const glm::vec4& tintColor)
	{
		if (s_RenderData.QuadIndexCount >= Renderer2DStorage::MaxIndices)
		{
			FlushAndReset();
		}
		float textureID = 0.0f;
		float tillingFactor = tilling;

		glm::vec4 color(1.0f);

		for (int i = 0; i < s_RenderData.TextureSlotIndex; i++)
		{
			if (*s_RenderData.TextureSlots[i] == *texture)
			{
				textureID = i;
				break;
			}
		}

		if (textureID == 0.0f)
		{
			textureID = s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex++] = texture;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[0];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[1];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,0.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[2];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 1.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[3];
		s_RenderData.QuadVertexBufferPtr->Color = color;
		s_RenderData.QuadVertexBufferPtr->Texcoord = { 0.0f,1.0f };
		s_RenderData.QuadVertexBufferPtr->TextureID = textureID;
		s_RenderData.QuadVertexBufferPtr->TillingFactor = tillingFactor;
		s_RenderData.QuadVertexBufferPtr++;

		s_RenderData.QuadIndexCount += 6;

		s_RenderData.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		s_RenderData.Stats.DrawCalls = 0;
		s_RenderData.Stats.QuadCount = 0;
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_RenderData.Stats;
	}
}