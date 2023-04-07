#include "pkpch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "PKEngine/Scene/Actor.h"
namespace PKEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> scene)
	{
		SetContext(scene);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{

	} 

	void SceneHierarchyPanel::SetContext(const Ref<Scene> scene)
	{
		m_SceneContext = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		auto actors = m_SceneContext->GetAllActors();
		
		ImGui::Begin("SceneHierarchy");
		
		for (auto actor : actors)
		{
			DrawTreeNode(actor);
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawTreeNode(Ref<Actor> actor)
	{
		ImGuiTreeNodeFlags flag = ((actor.get() == m_SelectedActor.get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool bopen = ImGui::TreeNodeEx(actor->GetName().c_str(), flag);
		if (ImGui::IsItemClicked())
		{
			m_SelectedActor = actor;
		}

		if(bopen)
		{
			ImGui::TreePop();
		}
	}
}