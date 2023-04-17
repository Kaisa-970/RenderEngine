#pragma once
#include "PKEngine/Renderer/FrameBuffer.h"

namespace PKEngine
{
	class OpenGLFrameBuffer:public FrameBuffer
	{
	public:
		OpenGLFrameBuffer() = default;
		OpenGLFrameBuffer(const FrameBufferParams& params);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; };
		virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttachment; };

		virtual const FrameBufferParams& GetFrameBufferParams() const override { return m_FrameBufferParams; }

		void Resize();

	private:
		uint32_t m_RendererID = 0;
		FrameBufferParams m_FrameBufferParams;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};

}