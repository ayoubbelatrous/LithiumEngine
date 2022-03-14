#include "lipch.h"
#include "InspectorPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "entt.hpp"
#include "Scene/Components.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Core/Base.h"
#include "AssetManager/AssetManager.h"
#include <filesystem>
#include <iostream>
#include "Script/ScriptObject.h"
#include "Core/Application.h"
namespace Lithium
{
	extern const std::filesystem::path root;
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	static bool Property(const char* label, std::string& value, bool error = false)
	{
		bool modified = false;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, value.c_str(), sizeof(buffer));
		if (error)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
			
		}
		if (ImGui::InputText("Module", buffer, 256))
		{
			value = std::string(buffer);
			modified = true;
		}
		if (error)
			ImGui::PopStyleColor();
		return modified;
	}

	static bool Property(const std::string& name,int* value)
	{
		bool modified = false;
		
		if (ImGui::DragInt(name.c_str(), value,0.1f))
		{
			modified = true;
		}

		return modified;
	}
	static bool Property(const std::string& name, float* value)
	{
		bool modified = false;
		if (ImGui::DragFloat(name.c_str(), value))
		{
			modified = true;
		}

		return modified;
	}
	static bool Property(const std::string& name, glm::vec2* value)
	{
		bool modified = false;
		if (ImGui::DragFloat2(name.c_str(), (float*)value))
		{
			modified = true;
		}

		return modified;
	}
	static bool Property(const std::string& name, glm::vec3* value)
	{
		bool modified = false;
		if (ImGui::DragFloat3(name.c_str(), (float*)value))
		{
			modified = true;
		}

		return modified;
	}

	static bool Property(const std::string& name, glm::vec4* value)
	{
		bool modified = false;
		if (ImGui::DragFloat4(name.c_str(), (float*)value))
		{
			modified = true;
		}

		return modified;
	}

	static bool Property(const std::string& name, std::string& value)
	{
		bool modified = false;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, value.c_str(), sizeof(buffer));
		if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
		{
			value = std::string(buffer);
			modified = true;
		}

		return modified;
	}

	static bool Property(const std::string& name, UUID& id)
	{

		bool modified = false;

		std::string EntityName;

		if (id != 0)
		{
			Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[id], Application::Get().sceneManager->GetActiveScene().get());
			EntityName = entity.GetComponent<NameComponent>().GetName();
		}
		
	
		ImGui::Text("%s", name.c_str());
		ImGui::SameLine();
		ImGui::Button(EntityName.c_str(), {100,20});
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
			{
				const UUID* uuid = (UUID*)payload->Data;
				id = *uuid;
				modified = true;
			}
			else
			{
				modified = false;
			}
			ImGui::EndDragDropTarget();
		}
		return modified;
	}

	void InspectorPanel::OnCreate()
	{

	}

	void InspectorPanel::OnUpdate()
	{


		ImGui::Begin("Inspector");

		if (m_Selection.GetHandle() != entt::null && m_Selection.getScene() != nullptr)
		{
			
			if (m_Selection.HasComponent<NameComponent>())
			{
				NameComponent& namec = m_Selection.GetComponent<NameComponent>();

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, namec.GetName().c_str(), sizeof(buffer));
				if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
				{
					namec.GetName() = std::string(buffer);
				}

			}

			if (m_Selection.HasComponent<TransformComponent>())
			{

				ImGui::Selectable("Transform");

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
					}
					if (ImGui::MenuItem("Copy Component"))
					{
					}

					ImGui::EndPopup();
				}

				ImGui::Separator();
				DrawVec3Control("Position", m_Selection.GetComponent<TransformComponent>().Position);
				ImGui::Separator();
				DrawVec3Control("Rotation", m_Selection.GetComponent<TransformComponent>().Rotation);
				ImGui::Separator();
				DrawVec3Control("Scale", m_Selection.GetComponent<TransformComponent>().Scale);
				ImGui::Separator();
			}

			if (m_Selection.HasComponent<SpriteRendererComponent>())
			{
				ImGui::Selectable("Sprite Renderer");


				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
					}
					if (ImGui::MenuItem("Copy Component"))
					{
					}

					ImGui::EndPopup();
				}



				ImGui::Separator();
				ImGui::ColorEdit4("Color", glm::value_ptr(m_Selection.GetComponent<SpriteRendererComponent>().Color));
				if (m_Selection.GetComponent<SpriteRendererComponent>().tex->loaded)
				{
					ImGui::Image((ImTextureID)m_Selection.GetComponent<SpriteRendererComponent>().tex->GetID(), { 75,75 });
				}
				else
				{
					ImGui::Button("Texture", { ImGui::GetContentRegionAvail().x,50 });
				}


				if (ImGui::BeginDragDropTarget())
				{

					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturepath = root / path;
						std::filesystem::path _path = m_Selection.GetComponent<SpriteRendererComponent>().tex->GetPath();


						//CORE_LOG(_path);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::InputFloat2("Tex Index", glm::value_ptr(m_Selection.GetComponent<SpriteRendererComponent>().texIndex));
			}


			if (m_Selection.HasComponent<ScriptGroupeComponent>())
			{
			
				ScriptGroupeComponent& scriptGroupe = m_Selection.GetComponent<ScriptGroupeComponent>();
				for (auto& script : scriptGroupe.Scripts)
				{
					const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
					bool open = ImGui::TreeNodeEx((void*)(std::hash<uint64_t>{}(script.uuid)), treeNodeFlags, script.Name.c_str());
					
					ImGui::PopStyleVar();
					if (open)
					{
						std::string ModuleName = script.Name;
						if (Property("Module",ModuleName,!script.Loaded))
						{
							script.Name = ModuleName;
							script.Loaded = false;
						}
						if (script.Loaded)
						{

							for (auto& field : script.Scriptobject->GetFields())
							{

								switch (field.second->GetType())
								{
								case (ScriptType::Int):
								{
									int val = field.second->GetValue<int>();
									if (Property(field.first, &val))
									{
										field.second->SetValue<int>(val);
									}
									break;
								}
								case (ScriptType::Float):
								{
									float val = 0;
									val = field.second->GetValue<float>();
									if (Property(field.first, &val))
									{
										field.second->SetValue(val);
									}
									break;
								}
								case (ScriptType::Vec2):
								{
									glm::vec2 val = glm::vec2(0);
									val = field.second->GetValue<glm::vec2>();
									if (Property(field.first, &val))
									{
										field.second->SetValue(val);
									}
									break;
								}


								case (ScriptType::Vec3):
								{
									glm::vec3 val = glm::vec3(0);
									val = field.second->GetValue<glm::vec3>();
									if (Property(field.first, &val))
									{
										field.second->SetValue(val);
									}
									break;
								}


								case (ScriptType::Vec4):
								{
									glm::vec4 val = glm::vec4(0);
									val = field.second->GetValue<glm::vec4>();
									if (Property(field.first, &val))
									{
										field.second->SetValue(val);
									}
									break;
								}

								case (ScriptType::String):
								{

									std::string val = field.second->GetValue<std::string>();
									if (Property(field.first, val))
									{
										field.second->SetValue(val);
									}
									break;
								}
								case (ScriptType::Entity):
								{

									UUID val = field.second->GetValue<uint64_t>();
									if (Property(field.first, val))
									{
										field.second->SetValue((uint64_t)val);
									}
									break;
								}
								}

							}

						}
					  ImGui::TreePop();
					}
				}
			
				
			
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_Selection.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem("Camera"))
					{
						m_Selection.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_Selection.HasComponent<SpriteRendererComponent>())
				{
					if (ImGui::MenuItem("Sprite Renderer"))
					{
						m_Selection.AddComponent<SpriteRendererComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

			
				if (ImGui::MenuItem("Script"))
				{
					if (!m_Selection.HasComponent<ScriptGroupeComponent>())
						m_Selection.AddComponent<ScriptGroupeComponent>();
					ScriptComponent script = ScriptComponent();
					m_Selection.GetComponent<ScriptGroupeComponent>().AddScript("");
					ImGui::CloseCurrentPopup();
				}
				


				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}

