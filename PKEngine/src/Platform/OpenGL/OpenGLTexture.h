#pragma once
#include "glad/glad.h"
#include "PKEngine/Renderer/Texture.h"

//struct GLenum;
namespace PKEngine {
	class OpenGLTexture :public Texture2D {
	public:
		OpenGLTexture() = default;
		OpenGLTexture(uint32_t width,uint32_t height);
		OpenGLTexture(const std::string& path);
		~OpenGLTexture();
		virtual uint32_t GetWidth()const override { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }

		void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& otherTexture) const override
		{
			return m_RendererID == ((OpenGLTexture&)otherTexture).m_RendererID;
		}

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}