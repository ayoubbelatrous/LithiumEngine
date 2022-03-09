#include "lipch.h"

#include "SceneHierachyPanel.h"
#include "imgui/imgui.h"
#include "entt.hpp"
#include "Scene/Components.h"

namespace Lithium
{

	void SceneHierachyPanel::OnCreate()
	{
		
	}

	void SceneHierachyPanel::OnUpdate()
	{
		ImGui::Begin("Hierachy");
		
		_Scene->GetRegistry().each([&](auto entityID)
		{
				Entity entity{ entityID , _Scene.get() };
				DrawEntity(entity);
		});
		ImGui::End();
	}
	
	 void SceneHierachyPanel::DrawEntity(Entity entity) {
		/*
		 if(entity.HasComponent<ChildManagerComponent>())
		 if (entity.GetComponent<ChildManagerComponent>().Children.empty())
			 return;
		*/
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
		auto& tag = entity.GetComponent<NameComponent>().Name;
		ImGuiTreeNodeFlags flags = ((_Selection.GetHandle() == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));
		
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.GetHandle(), flags, tag.c_str());
		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			_Selection = entity;
		}
		if (ImGui::BeginDragDropSource())
		{
			UUID id = entity.GetComponent<IDComponent>().ID;
			ImGui::SetDragDropPayload("ENTITY", &id, sizeof(UUID));
			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		

		
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}


		if (entityDeleted)
		{
			_Scene->DeleteEntity(entity);
			_Selection = Entity();
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