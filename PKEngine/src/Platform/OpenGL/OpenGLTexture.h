#pragma once

#include "PKEngine/Renderer/Texture.h"

namespace PKEngine {
	class OpenGLTexture :public Texture2D {
	public:
		OpenGLTexture() = default;
		OpenGLTexture(const std::string& path);
		~OpenGLTexture();
		virtual uint32_t GetWidth()const override { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}