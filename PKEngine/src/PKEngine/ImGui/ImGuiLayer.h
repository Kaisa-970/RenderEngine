#pragma once
#include "../Layer.h"
#include "PKEngine/Core.h"
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
		virtual void OnUpdate() override;


		void Begin();
		void End();
	private:
		float m_Time;
	};
}