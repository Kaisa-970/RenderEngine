#include "pkpch.h"
#include "PerspectiveCamera.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
namespace PKEngine {

	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar)
		:m_Fov(fovy),m_AspectRatio(aspect),m_Near(znear),m_Far(zfar), m_ProjectionMatrix(glm::perspective(fovy, aspect, znear, zfar)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec3 PerspectiveCamera::GetForward() const
	{
		return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 PerspectiveCamera::GetRight() const
	{
		return glm::cross(GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//m_ViewMatrix = glm::inverse(transform);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::mat4_cast(glm::quat(glm::radians(m_Rotation)));
		m_ViewMatrix = glm::inverse(transform); 

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PK_BIND_EVENT_FN(PerspectiveCamera::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(PK_BIND_EVENT_FN(PerspectiveCamera::OnWindowResize));
	}
	void PerspectiveCamera::SetFov(float fov)
	{
		 m_Fov = glm::radians(fov); 
		 m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		 RecalculateViewMatrix();
	}
	void PerspectiveCamera::SetAspect(float aspect)
	{
		m_AspectRatio = aspect;
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		RecalculateViewMatrix();
	}
	bool PerspectiveCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		RecalculateViewMatrix();
		return false;
	}
	bool PerspectiveCamera::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = e.GetWidth() * 1.0f / (e.GetHeight() * 1.0f);
		//float sw = e.GetWidth() / 1280.0f;
		float sh = e.GetHeight() / 1080.0f;
		//float tmpZoom = m_ZoomLevel * sh;
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		RecalculateViewMatrix();
		return false;
	}
}