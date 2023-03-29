#include "pkpch.h"
#include "OrthoCameraController.h"
#include "PKEngine/Input.h"
#include "PKEngine/KeyCodes.h"

namespace PKEngine {
	OrthoCameraController::OrthoCameraController(float aspect,bool brot)
		:m_Aspect(aspect),m_Camera(-aspect*m_ZoomLevel, aspect * m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel),m_CanRot(brot)
	{

	}
	OrthoCameraController::~OrthoCameraController()
	{

	}
	void OrthoCameraController::Update(float ts)
	{
		float deltaTime = ts;
		if (Input::IsKeyPressed(PK_KEY_W)) {
			m_Position.y += deltaTime * m_MoveSpeed;
		}
		else if (Input::IsKeyPressed(PK_KEY_S)) {
			m_Position.y -= deltaTime * m_MoveSpeed;
		}

		if (Input::IsKeyPressed(PK_KEY_A)) {
			m_Position.x -= deltaTime * m_MoveSpeed;
		}
		else if (Input::IsKeyPressed(PK_KEY_D)) {
			m_Position.x += deltaTime * m_MoveSpeed;
		}

		if (m_CanRot) {
			if (Input::IsMouseButtonPressed(0)) {
				m_Rotation -= deltaTime * m_RotSpeed;
			}
			else if (Input::IsMouseButtonPressed(1)) {
				m_Rotation += deltaTime * m_RotSpeed;
			}

			m_Camera.SetRotation(m_Rotation);
		}

		m_Camera.SetPosition(m_Position);
	}
	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PK_BIND_EVENT_FN(OrthoCameraController::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(PK_BIND_EVENT_FN(OrthoCameraController::OnWindowResize));
	}
	bool OrthoCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.15f);
		m_Camera.SetProjectionMatrix(-m_Aspect * m_ZoomLevel, m_Aspect * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_Aspect = e.GetWidth() * 1.0f / (e.GetHeight() * 1.0f);
		m_Camera.SetProjectionMatrix(-m_Aspect * m_ZoomLevel, m_Aspect * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}