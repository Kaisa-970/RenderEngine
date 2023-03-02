#pragma once
#include "PKEngine/Core.h"
namespace PKEngine {
	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}
