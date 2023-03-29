#pragma once
#include "PKEngine/Core/Layer.h"
#include "PKEngine/Core/Core.h"
#include <PKEngine/Events/MouseEvent.h>
#include <PKEngine/Events/KeyEvent.h>
#include "PKEngine/Events/ApplicationEvent.h"

namespace PKEngine {
	class PKENGINE_API ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time;
	};
}