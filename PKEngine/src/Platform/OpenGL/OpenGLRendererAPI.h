#pragma once
#include "PKEngine/Renderer/RendererAPI.h"
namespace PKEngine {
	class OpenGLRendererAPI : public RendererAPI {

	public:
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Init() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0) override;
	};
}