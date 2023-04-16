#pragma once
#include "glm/glm.hpp"

namespace PKEngine
{
	class LightComponent
	{
	public:
		LightComponent() = default;
		virtual ~LightComponent() = default;
		float GetIntensity() const { return m_LightIntensity; }
		glm::vec3 GetColor() const { return m_LightColor; }

		void SetIntensity(float intensity) { m_LightIntensity = intensity; }
		void SetColor(glm::vec3 color) { m_LightColor = color; }
	private:
		glm::vec3 m_LightColor = glm::vec3(1.0f);
		float m_LightIntensity = 3.0f;
	};
}