#pragma once
#include <PKEngine/Renderer/OrthographicCamera.h>
#include "PKEngine/Events/ApplicationEvent.h"
#include "PKEngine/Events/MouseEvent.h"
#include "glm/glm.hpp"

namespace PKEngine {
	class OrthoCameraController
	{
	public:
		OrthoCameraController() = default;
		OrthoCameraController(float aspect,bool brot = false);
		~OrthoCameraController();

		void Update(float ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		bool m_CanRot;
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		float m_MoveSpeed = 1.0f;
		float m_Rotation = 0.0f;
		float m_RotSpeed = 10.0f;
	};
}