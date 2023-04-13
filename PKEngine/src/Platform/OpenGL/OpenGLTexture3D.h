#pragma once
#include "PKEngine/Renderer/Texture.h"

namespace PKEngine
{
	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D() = default;
		OpenGLTexture3D(const std::vector<std::string>& path);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth()const override { return m_Width; }
		virtual uint32_t GetHeight()const override { return m_Height; }

		void SetData(void* data, uint32_t size) override;

		virtual uint32_t GetRenderID() const override { return m_RendererID; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool operator==(const Texture& otherTexture) const override
		{
			return m_RendererID == ((OpenGLTexture3D&)otherTexture).m_RendererID;
		}

	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
	};

}