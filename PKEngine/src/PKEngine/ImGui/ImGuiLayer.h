#pragma once
#include "../Layer.h"
#include "PKEngine/Core.h"

namespace PKEngine {
	class PKENGINE_API ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;

	private:
		float m_Time;
	};
}