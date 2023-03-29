#pragma once

#include "glm/glm.hpp"

namespace PKEngine {
	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection){}
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}