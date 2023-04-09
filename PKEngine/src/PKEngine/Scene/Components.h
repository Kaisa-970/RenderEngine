#pragma once
#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

struct TransformComponent
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 GetMatrix()
	{
		return glm::translate(glm::mat4(1.0f),Position)
			* glm::rotate(glm::mat4(1.0f),glm::radians(Rotation.y), glm::vec3(0.0f,1.0f,0.0f))
			* glm::scale(glm::mat4(1.0f), Scale);
	}

	TransformComponent() = default;
	TransformComponent(const TransformComponent& other) = default;
	TransformComponent(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0.0f),const glm::vec3& scale = glm::vec3(1.0f))
		:Position(pos),Rotation(rot),Scale(scale)
	{}
};

struct SpriteComponent
{
	glm::vec4 Color;

	SpriteComponent() = default;
	SpriteComponent(const SpriteComponent& other) = default;
	SpriteComponent(const glm::vec4& col)
		:Color(col)
	{}
};
