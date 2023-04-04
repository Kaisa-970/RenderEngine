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
		virtual uint32_t GetColorAttachmentID() const { return m_ColorAttachment; };

		virtual const FrameBufferParams& GetFrameBufferParams() const override { return m_FrameBufferParams; }

		void Resize();

	private:
		uint32_t m_RendererID;
		FrameBufferParams m_FrameBufferParams;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
	};

}