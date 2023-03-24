#include "pkpch.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PKEngine {
	Mesh::Mesh(const std::string& filename)
	{
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
		PK_CORE_INFO("Mesh Create!");
	}
	Mesh::~Mesh()
	{

	}
}