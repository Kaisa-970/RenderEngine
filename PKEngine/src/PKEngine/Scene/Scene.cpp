#include "pkpch.h"
#include "Scene.h"

namespace PKEngine {
	Scene::Scene()
	{
		struct TransformComponent
		{
			float x;
			float y;
		};
		auto entity = m_Registry.create();

	}

	Scene::~Scene()
	{

	}
}