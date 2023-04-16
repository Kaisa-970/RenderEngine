#include "pkpch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "PKEngine/Scene/Actor.h"
#include <PKEngine/Scene/LightComponent.h>
#include <PKEngine/Scene/MeshComponent.h>
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedActor = nullptr;
		}
		ImGui::End();



		ImGui::Begin("Properties");
		if (m_SelectedActor)
		{
			DrawComponents(m_SelectedActor);
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opd = ImGui::TreeNodeEx((void*)41478, flags, actor->GetName().c_str());
			if (opd)
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	void SceneHierarchyPanel::DrawComponents(Ref<Actor> actor)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, actor->GetName().c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			actor->SetName(std::string(buffer));
		}
		if (actor->HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				auto& transform = actor->GetComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
				ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);
				ImGui::TreePop();
			}
		}

		if (actor->HasComponent<MeshComponent>())
		{
			if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& mesh = actor->GetComponent<MeshComponent>();
				auto shader = mesh.GetMaterial();
				auto& sps = mesh.GetShaderParameters();
				ImGui::SliderFloat("Roughness", &sps.Roughness, 0, 1);
				ImGui::SliderFloat("Metallic", &sps.Metallic, 0, 1);

				ImGui::TreePop();
			}
		}

		if (actor->HasComponent<LightComponent>())
		{
			if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& light = actor->GetComponent<LightComponent>();
				glm::vec3 color = light.GetColor();
				ImGui::ColorEdit3("Color", &color[0]);
				light.SetColor(color);
				float inten = light.GetIntensity();
				ImGui::SliderFloat("Intensity", &inten,0.01f,10.0f);
				light.SetIntensity(inten);

				ImGui::TreePop();
			}
		}
	}
}