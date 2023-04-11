#pragma once

#include "glm/glm.hpp"

namespace PKEngine {
	class PerspectiveCamera {
	public:
		PerspectiveCamera();
		PerspectiveCamera(float fovy, float aspect, float znear, float zfar);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		glm::vec3 GetRotation() const { return m_Rotation; }
		//const glm::vec3& GetForwardDir() const { return m_Position; }

		glm::vec3 GetForward() const;
		glm::vec3 GetRight() const;

		inline const glm::mat4& GetViewMatrix()const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix()const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix()const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};
}