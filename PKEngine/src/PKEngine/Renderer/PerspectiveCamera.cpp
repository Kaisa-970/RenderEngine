#include "pkpch.h"
#include "PerspectiveCamera.h"
#include "glm/gtx/transform.hpp"
namespace PKEngine {

	PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar)
		:m_ProjectionMatrix(glm::perspective(fovy, aspect, znear, zfar)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//m_ViewMatrix = glm::inverse(transform);

		glm::mat4 transform = glm::lookAt(m_Position,glm::vec3(0), glm::vec3(0.0f,1.0f,0.0f));
		m_ViewMatrix = transform; 

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}