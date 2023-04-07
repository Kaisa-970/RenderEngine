#pragma once
#include "PKEngine/Scene/Scene.h"

namespace PKEngine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene> scene);
		~SceneHierarchyPanel();

		void SetContext(const Ref<Scene> scene);

		void OnImGuiRender();
		void DrawTreeNode(Ref<Actor> actor);

	private:
		Ref<Scene> m_SceneContext;
		Ref<Actor> m_SelectedActor;
	};

}