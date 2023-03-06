#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include <PKEngine/Renderer/Shader.h>
namespace PKEngine {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray,const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}