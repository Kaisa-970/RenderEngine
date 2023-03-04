#pragma once

#include "Core.h"
#include <PKEngine/Window.h>
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/LayerStack.h"

#include "PKEngine/ImGui/ImGuiLayer.h"
#include <PKEngine/Renderer/Shader.h>
#include "PKEngine/Renderer/Buffer.h"

namespace PKEngine {
	class PKENGINE_API Application
	{
	public:
		Application();

		virtual ~Application();

		static inline Application& Get() { return *s_Instance; }

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	// To be defined in Client
	Application* CreateApplication();
}

