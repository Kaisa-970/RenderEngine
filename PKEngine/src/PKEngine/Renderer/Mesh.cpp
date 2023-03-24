#include "pkpch.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>

#include <assimp/postprocess.h>

namespace PKEngine {
	Mesh::Mesh(const std::string& filename)
	{
		Assimp::Importer importer;
		
		m_Scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (m_Scene->HasMeshes()) {
			
			auto mesh0 = m_Scene->mMeshes[0];
			m_verticesCount = mesh0->mNumVertices;
			auto vertices = mesh0->mVertices;

			//unsigned int nCount = mesh0->n
			auto normals = mesh0->mNormals;
			auto uvs = mesh0->mTextureCoords[0];
			m_Vertices = new float[m_verticesCount * (3 * 2 + 2)];
			for (size_t i = 0; i < m_verticesCount; i++)
			{
				size_t idx = 8 * i;
				m_Vertices[idx] = vertices[i].x;
				m_Vertices[idx +1] = vertices[i].y;
				m_Vertices[idx +2] = vertices[i].z;
				m_Vertices[idx + 3] = uvs[i].x;
				m_Vertices[idx + 4] = uvs[i].y;
				m_Vertices[idx + 5] = normals[i].x;
				m_Vertices[idx + 6] = normals[i].y;
				m_Vertices[idx + 7] = normals[i].z;
				//PK_CORE_INFO("Vertex{0} = ({1},{2},{3})!", i, m_Vertices[idx], m_Vertices[idx + 1], m_Vertices[idx + 2]);
			}
			m_FaceCount = mesh0->mNumFaces;
			auto faces = mesh0->mFaces;
			m_Faces = new unsigned int[m_FaceCount * 3];
			//m_Faces = faces->mIndices;
			for (size_t i = 0; i < m_FaceCount; i++)
			{
				size_t idx = 3 * i;
				auto facei = faces[i];

				for (size_t j = 0; j < facei.mNumIndices; j++)
				{
					
					m_Faces[idx + j] = facei.mIndices[j];
				}
				//PK_CORE_INFO("Face{0} = ({1},{2},{3})!",i, m_Faces[idx], m_Faces[idx + 1], m_Faces[idx + 2]);
			}
			
			PK_CORE_INFO("Load Mesh:Vertices count = {0}, Faces count = {1}!", m_verticesCount, m_FaceCount);
		}
		

	}
	Mesh::~Mesh()
	{
		//delete m_Scene;
		delete[] m_Vertices;
		delete[] m_Faces;
	}
}