#include "pkpch.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PKEngine {
	Mesh::Mesh(const std::string& filepath):m_Path(filepath)
	{
		Assimp::Importer importer;
		PK_CORE_INFO("Create Mesh");
		//importer.ReadFile
	}
	Mesh::~Mesh()
	{
	}
}