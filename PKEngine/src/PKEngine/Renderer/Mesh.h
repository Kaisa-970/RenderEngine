#pragma once
#pragma once

#include <assimp/scene.h>

namespace PKEngine {
	class Mesh {
	public:
		Mesh() = default;
		Mesh(const std::string& filename);
		~Mesh();

		inline float* GetVertices() { return m_Vertices; }
		inline unsigned int* GetIndices() { return m_Faces; }

		inline int GetVerticesCount(){ return m_verticesCount; }
		inline int GetFacesCount() { return m_FaceCount; }

	private:
		const aiScene* m_Scene;
		float* m_Vertices;
		unsigned int* m_Faces;
		unsigned int m_verticesCount;
		unsigned int m_FaceCount;
	};
}