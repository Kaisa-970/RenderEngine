#pragma once
#include "PKEngine/Core.h"
#include <PKEngine/Events/Event.h>

namespace PKEngine {
	class PKENGINE_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& e) {};

		inline const std::string& GetName()const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}