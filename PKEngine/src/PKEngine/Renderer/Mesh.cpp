#include "pkpch.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace PKEngine {
	void Mesh::Load(const std::string& filename)
	{
		Assimp::Importer import;
	}
}