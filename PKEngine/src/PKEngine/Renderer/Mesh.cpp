#include "pkpch.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>

#include <assimp/postprocess.h>

namespace PKEngine {
	Mesh::Mesh(const std::string& filename)
	{
		Assimp::Importer importer;
		
		auto m_Scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
		
		if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode)
		{
			PK_CORE_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
			//PK_CORE_ASSERT(false, "Import mesh failed!");
			return;
		}
		if (m_Scene->HasMeshes()) 
		{
			auto mesh0 = m_Scene->mMeshes[0];

			m_VerticesCount = mesh0->mNumVertices;
			m_Vertices = new Vertex[m_VerticesCount];

			auto vertices = mesh0->mVertices;
			auto normals = mesh0->mNormals;
			auto uvs = mesh0->mTextureCoords[0];

			for (size_t i = 0; i < m_VerticesCount; i++)
			{
				m_Vertices[i].Position = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
				m_Vertices[i].Texcoord = glm::vec2(uvs[i].x, uvs[i].y);
				m_Vertices[i].Normal = glm::vec3(normals[i].x, normals[i].y, normals[i].z);
			}

			m_TriangleCount = mesh0->mNumFaces;
			m_Triangles = new Triangle[m_TriangleCount];

			auto faces = mesh0->mFaces;
			//m_Faces = faces->mIndices;
			for (size_t i = 0; i < m_TriangleCount; i++)
			{
				m_Triangles[i].Indices = glm::uvec3(faces[i].mIndices[0], faces[i].mIndices[1], faces[i].mIndices[2]);

			}
			
			PK_CORE_INFO("Load Mesh:Vertices count = {0}, Triangles count = {1}!", m_VerticesCount, m_TriangleCount);
			
			SetRenderData();
		}
	}

	Mesh::~Mesh()
	{
		//delete m_Scene;
		delete[] m_Vertices;
		delete[] m_Triangles;
	}

	void Mesh::SetRenderData()
	{
		m_VertexArray = PKEngine::VertexArray::Create();
		PKEngine::Ref<PKEngine::VertexBuffer> m_MeshBuffer;
		m_MeshBuffer.reset(PKEngine::VertexBuffer::Create((float*)m_Vertices, m_VerticesCount * sizeof(Vertex)));

		PKEngine::BufferLayout meshlayout = {
				{PKEngine::ShaderDataType::Float3,"a_Position"},
				{PKEngine::ShaderDataType::Float2,"a_Texcoord"},
				{PKEngine::ShaderDataType::Float3,"a_Normal"}
		};
		m_MeshBuffer->SetLayout(meshlayout);

		m_VertexArray->AddVertexBuffer(m_MeshBuffer);

		PKEngine::Ref<PKEngine::IndexBuffer> m_MeshIndexBuffer;
		m_MeshIndexBuffer.reset(PKEngine::IndexBuffer::Create((uint32_t*)m_Triangles, m_TriangleCount * 3));
		m_VertexArray->SetIndexBuffer(m_MeshIndexBuffer);
	}
}