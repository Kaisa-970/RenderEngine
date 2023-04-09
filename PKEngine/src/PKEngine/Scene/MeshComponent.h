#pragma once
#include "PKEngine/Renderer/Mesh.h"
#include "PKEngine/Renderer/Shader.h"

namespace PKEngine
{
	class MeshComponent
	{
	public:
		MeshComponent() = default;
		MeshComponent(Ref<Mesh> mesh,Ref<Shader> shader);
		~MeshComponent() = default;

		void SetMesh(Ref<Mesh> mesh) { m_Mesh = mesh; };
		void SetMaterial(Ref<Shader> shader) { m_Shader = shader; };

		Ref<Mesh> GetMesh() const { return m_Mesh; };
		Ref<Shader> GetMaterial() const { return m_Shader; };

	private:
		Ref<Mesh> m_Mesh;
		Ref<Shader> m_Shader;

	};
}