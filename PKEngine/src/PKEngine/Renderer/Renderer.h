#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include <PKEngine/Renderer/Shader.h>
#include <PKEngine/Renderer/PerspectiveCamera.h>
#include "PKEngine/Renderer/Mesh.h"

namespace PKEngine {

	class Renderer {
	public:
		static void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void SetViewport(uint32_t width, uint32_t height);

		static void Submit(const Ref<VertexArray>& vertexArray,const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static void Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		//static void DrawSky(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void SetViewProjectionMatrix(const glm::mat4& vpmat);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}