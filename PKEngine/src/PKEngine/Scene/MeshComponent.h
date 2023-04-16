#pragma once
#include "PKEngine/Renderer/Mesh.h"
#include "PKEngine/Renderer/Shader.h"

namespace PKEngine
{
	struct ShaderParameters
	{
		float Roughness;
		float Metallic;
	};

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

		ShaderParameters& GetShaderParameters() { return m_ShaderParam; }
		void SetShaderParameters(const ShaderParameters& sps) { m_ShaderParam = sps; }
	private:
		Ref<Mesh> m_Mesh;
		Ref<Shader> m_Shader;
		ShaderParameters m_ShaderParam{ 0.5f,0.0f };
	};
}