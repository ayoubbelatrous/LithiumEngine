#include "SceneHierachyPanel.h"
#include "imgui/imgui.h"
#include "entt.hpp"
#include "Scene/Components.h"

namespace Lithium
{

	void SceneHierachyPanel::OnCreate()
	{
		
	}

	void SceneHierachyPanel::OnUpdate(Entity& _Selection)
	{
		ImGui::Begin("Hierachy");
		

	
		_Scene->GetRegistry().each([&](auto entityID)
			{
				Entity entity{ entityID , _Scene.get() };
				DrawEntity(entity,_Selection);
			});

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("create entity"))
				{
					Entity entity = _Scene->CreateEntity("new entity");

					entity.AddComponent<TransformComponent>();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::End();
	}




	 void SceneHierachyPanel::DrawEntity(Entity entity,Entity& _Selection) {


		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
		auto& tag = entity.GetComponent<NameComponent>().Name;
		ImGuiTreeNodeFlags flags = ((_Selection.GetHandle() == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, tag.c_str());
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		if (ImGui::IsItemClicked())
		{
			_Selection = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
				_Scene->DeleteEntity(entity);
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		ImGui::PopStyleColor();
	}
}