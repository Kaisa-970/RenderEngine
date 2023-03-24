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
		if (scene->HasMeshes()) {
			
			auto mesh0 = scene->mMeshes[0];
			auto vCount = mesh0->mNumVertices;
			PK_CORE_INFO("Load Mesh:Vertices count = {0}!",vCount);
		}
		

	}
	Mesh::~Mesh()
	{

	}
}