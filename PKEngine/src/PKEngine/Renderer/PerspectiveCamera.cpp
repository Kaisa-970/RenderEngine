#include "pkpch.h"
#include "PerspectiveCamera.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
namespace PKEngine {

	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar)
		:m_ProjectionMatrix(glm::perspective(fovy, aspect, znear, zfar)),
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
}