#pragma once

#include <assimp/scene.h>
#include "glm/glm.hpp"
#include "PKEngine/Renderer/VertexArray.h"

namespace PKEngine {
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 Texcoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
	};

	struct Triangle
	{
		glm::uvec3 Indices;
	};

	class Mesh {
	public:
		Mesh() = default;
		Mesh(const std::string& filename);
		Mesh(Ref<VertexArray> va);
		~Mesh();

		const Vertex* GetVertices() const{ return m_Vertices; }
		const Triangle* GetTriangles() const { return m_Triangles; }

		uint32_t GetVerticesCount()const{ return m_VerticesCount; }
		uint32_t GetTrianglesCount() const{ return m_TriangleCount; }

		Ref<VertexArray> GetVertexArray() const { return m_VertexArray; }
	
	private:
		void SetRenderData();

	private:
		Vertex* m_Vertices;
		Triangle* m_Triangles;
		uint32_t m_VerticesCount;
		uint32_t m_TriangleCount;
		Ref<VertexArray> m_VertexArray;
	};
}