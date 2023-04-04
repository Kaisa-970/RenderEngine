#pragma once
#include "PKEngine/Core/Core.h"
namespace PKEngine
{
	struct FrameBufferParams
	{
		uint32_t Width;
		uint32_t Height;

		uint32_t Samples = 1;

		bool SwapchainUse = false;
	};

	class FrameBuffer
	{
	public:
		//FrameBuffer();
		//FrameBuffer(const FrameBufferParams& params);
		//~FrameBuffer();

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentID() const = 0;

		virtual const FrameBufferParams& GetFrameBufferParams() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferParams& params);
	};
}