#include "pkpch.h"
#include "MeshComponent.h"

namespace PKEngine
{
	MeshComponent::MeshComponent(Ref<Mesh> mesh,Ref<Shader> shader)
		:m_Mesh(mesh),m_Shader(shader)
	{

	}
}