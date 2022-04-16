#include "epch.h"

#include "SceneHierachyPanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "entt.hpp"
#include "Scene/Components.h"

namespace Lithium
{

	void SceneHierachyPanel::OnCreate()
	{
		
	}

	void SceneHierachyPanel::OnUpdate()
	{
		ImGui::Begin("Hierarchy");
		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));

		//bool opened = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("Scene")), flags, "Scene");
		bool opened = ImGui::TreeNode("Scene");
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				const UUID* uuid = (UUID*)payload->Data;
				Entity DroppedEntity(_Scene->GetUUIDMap()[*uuid], _Scene.get());
				UUID parent = DroppedEntity.GetComponent<RelationShipComponent>().Parent;
				Entity ParentEntity(_Scene->GetUUIDMap()[parent], _Scene.get());
				RelationShipComponent& parentRC = ParentEntity.GetComponent<RelationShipComponent>();
				for (size_t i = 0; i < parentRC.Children.size(); i++)
				{
					if (parentRC.Children[i] == DroppedEntity.GetComponent<IDComponent>().ID)
					{
						parentRC.Children.erase(std::next(parentRC.Children.begin(), i));
						break;
					}

				}

				DroppedEntity.GetComponent<RelationShipComponent>().Parent = 0;
			}
			ImGui::EndDragDropTarget();
		}
		if (opened)
		{

			_Scene->GetRegistry().each([&](auto entityID)
			{
					Entity entity{ entityID , _Scene.get() };
					if (entity.GetComponent<RelationShipComponent>().Parent == 0)
					{
						DrawEntity(entity);
					}
			});
			ImGui::TreePop();
			ImGui::PopStyleVar(2);

		}
		else {
			ImGui::PopStyleVar(2);

		}

		ImGui::End();
	}
	
	 void SceneHierachyPanel::DrawEntity(Entity entity) {
		
		
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
		auto& tag = entity.GetComponent<NameComponent>().Name;
		ImGuiTreeNodeFlags flags = ((_Selection.GetHandle() == entity.GetHandle()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 3.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 2.0f));
		
		

		bool opened = ImGui::TreeNodeEx((void*)entity.GetHandle(), flags, tag.c_str());
		if (ImGui::BeginDragDropSource())
		{
			UUID id = entity.GetComponent<IDComponent>().ID;
			ImGui::SetDragDropPayload("ENTITY", &id, sizeof(UUID));

			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				const UUID* uuid = (UUID*)payload->Data;
				Entity DroppedEntity(_Scene->GetUUIDMap()[*uuid], _Scene.get());
				Entity OldParentEntity(_Scene->GetUUIDMap()[DroppedEntity.GetComponent<RelationShipComponent>().Parent], _Scene.get());
				OldParentEntity.GetComponent<RelationShipComponent>().RemoveChild(DroppedEntity.GetComponent<IDComponent>().ID);
				DroppedEntity.GetComponent<RelationShipComponent>().Parent = entity.GetComponent<IDComponent>().ID;
				TransformComponent& childTransform = DroppedEntity.GetComponent<TransformComponent>();
				entity.GetComponent<RelationShipComponent>().AddChild(*uuid);
				
				TransformComponent& parentTransform = entity.GetComponent<TransformComponent>();
				childTransform.LocalPosition = childTransform.Position - parentTransform.Position;
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			_Selection = entity;
		}
	
		
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}


		

		

		RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();


		if (opened)
		{
// 			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
// 			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
// 			if (opened)
// 				ImGui::TreePop();
			for (auto child : rc.Children)
			{
				Entity childEntity(_Scene->GetUUIDMap()[child], _Scene.get());
				
				{	
					DrawEntity(childEntity);
				}
			
			}
			ImGui::TreePop();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		if (entityDeleted)
		{
			_Scene->DeleteEntity(entity);
			_Selection = Entity();
		}
	 }
}