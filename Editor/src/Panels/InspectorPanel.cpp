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

	static bool Property(const std::string& name, bool* value)
	{
		bool modified = false;
		if (ImGui::Checkbox(name.c_str(), value))
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
		
	
		
		ImGui::Button(EntityName.c_str(), {125,20});
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
		ImGui::SameLine();
		ImGui::Text("%s", name.c_str());
		return modified;
	}

	static bool Property(const std::string& name, UUID& id,bool audioclip)
	{

		bool modified = false;

		std::string AudioClipName;

		if (id != 0)
		{
			std::filesystem::path mpath = Application::Get().assetManager->GetAssetPath(id);
			AudioClipName = mpath.filename().string();
		}
		ImGui::Button(AudioClipName.c_str(), { 125,20 });
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path audiopath = root / path;
				Asset asset = Application::Get().assetManager->GetAssetFromPath<Ref<Texture>>(audiopath.string());
				id = asset.GetUUID();
				modified = true;
			}
			else
			{
				modified = false;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		ImGui::Text("%s", name.c_str());
		
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
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("TransformComponent")), treeNodeFlags, "TransformComponent");
				ImGui::PopStyleVar();

				if (open)
				{
					DrawVec3Control("Position", m_Selection.GetComponent<TransformComponent>().Position);
					DrawVec3Control("Rotation", m_Selection.GetComponent<TransformComponent>().Rotation);
					DrawVec3Control("Scale", m_Selection.GetComponent<TransformComponent>().Scale);
					ImGui::TreePop();
				}

				
			}

			if (m_Selection.HasComponent<SpriteRendererComponent>())
			{
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("SpriteRenderer")), treeNodeFlags, "SpriteRenderer");
				ImGui::PopStyleVar();
				bool remove = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						remove = true;
					}
					ImGui::EndPopup();
				}




				if (open)
				{

					ImGui::ColorEdit4("Color", glm::value_ptr(m_Selection.GetComponent<SpriteRendererComponent>().Color));
					int drawOrder = m_Selection.GetComponent<SpriteRendererComponent>().DrawOrder;

					if (ImGui::InputInt("Draw Order", &drawOrder))
					{
						m_Selection.GetComponent<SpriteRendererComponent>().DrawOrder = drawOrder;
						Application::Get().sceneManager->GetActiveScene()->SortScene();

					}


					/*if (m_Selection.GetComponent<SpriteRendererComponent>().TextureAsset)
					{
						ImGui::Image((ImTextureID)m_Selection.GetComponent<SpriteRendererComponent>().tex->GetID(), { 75,75 });
					}*/

					{
						ImGui::Button("Texture", { ImGui::GetContentRegionAvail().x,50 });
					}


					if (ImGui::BeginDragDropTarget())
					{

						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturepath = root / path;
							Asset asset = Application::Get().assetManager->GetAssetFromPath<Ref<Texture>>(texturepath.string());
							m_Selection.GetComponent<SpriteRendererComponent>().TextureAsset = asset;
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::TreePop();
				}
				if (remove == true)
				{
					m_Selection.RemoveComponent<SpriteRendererComponent>();
				}
				
			}

			if (m_Selection.HasComponent<Rigidbody2DComponent>())
			{
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("RigidBody 2D")), treeNodeFlags, "RigidBody 2D");
				ImGui::PopStyleVar();
				bool remove = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						remove = true;
					}
					ImGui::EndPopup();
				}
				if (open)
				{
					Rigidbody2DComponent& rb2d = m_Selection.GetComponent<Rigidbody2DComponent>();
					const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
					const char* currentBodyTypeString = bodyTypeStrings[(int)rb2d.Type];

					if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
					{
						for (int i = 0; i < 2; i++)
						{
							bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
							if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
							{
								currentBodyTypeString = bodyTypeStrings[i];
								rb2d.Type = (PhysicsBodyType)i;
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}
					ImGui::Checkbox("Fixed Rotation", &rb2d.FixedRotation);
					ImGui::TreePop();
				}
				
				if (remove == true)
				{
					m_Selection.RemoveComponent<Rigidbody2DComponent>();
				}

			}

			if (m_Selection.HasComponent<BoxCollider2DComponent>())
			{

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("BoxCollider 2D")), treeNodeFlags, "BoxCollider 2D");
				ImGui::PopStyleVar();
				bool remove = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						remove = true;
					}
					ImGui::EndPopup();
				}
				if (open)
				{
					BoxCollider2DComponent& rb2d = m_Selection.GetComponent<BoxCollider2DComponent>();
					ImGui::PushItemWidth(125);
					ImGui::DragFloat2("Offset", glm::value_ptr(rb2d.Offset));
					ImGui::DragFloat2("Size", glm::value_ptr(rb2d.Size));
					ImGui::DragFloat("Density", &rb2d.Density, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Friction", &rb2d.Friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &rb2d.Restitution, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution Threshold", &rb2d.RestitutionThreshold, 0.01f, 0.0f);
					ImGui::PopItemWidth();
					ImGui::TreePop();
				}
				if (remove == true)
				{
					m_Selection.RemoveComponent<BoxCollider2DComponent>();
				}

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
					bool remove = false;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Remove Component"))
						{
							remove = true;
						}
						ImGui::EndPopup();
					}
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
								ImGui::PushItemWidth(125);
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

								case (ScriptType::Bool):
								{
									bool val = 0;
									val = field.second->GetValue<bool>();
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

								case (ScriptType::AudioClip):
								{

									UUID val = field.second->GetValue<UUID>();
									if (Property(field.first, val,true))
									{
										
										field.second->SetValue<UUID>((UUID)val);
									}
									break;
								}
								}
								ImGui::PopItemWidth();

							}

						}
					  ImGui::TreePop();
					}
				}
			
				
			
			}
			if (m_Selection.HasComponent<CameraComponent>())
			{

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("Camera")), treeNodeFlags, "Camera");
				ImGui::PopStyleVar();
				bool remove = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						remove = true;
					}
					ImGui::EndPopup();
				}
				if (open)
				{
					CameraComponent& camera = m_Selection.GetComponent<CameraComponent>();

					ImGui::Checkbox("Fixed Aspect Ration", &camera.FixedAspectRatio);
					ImGui::Checkbox("Primary", &camera.Primary);
					const char* ProjectionTypeString[] = { "Orthographic", "Perspective"};

					const char* CurrentProjectionString = ProjectionTypeString[(int)camera.Camera.GetProjectionType()];
					if (ImGui::BeginCombo("Projection", CurrentProjectionString))
					{
						for (int i = 0; i < 2; i++)
						{
							bool isSelected = CurrentProjectionString == ProjectionTypeString[i];
							if (ImGui::Selectable(ProjectionTypeString[i], isSelected))
							{
								CurrentProjectionString = ProjectionTypeString[i];
								camera.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
								
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}
					if (camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
					{
						float orthosize = camera.Camera.GetOrthographicSize();
						if (ImGui::DragFloat("Orthographic Size",&orthosize,0.1f))
						{
							camera.Camera.SetOrthographicSize(orthosize);
						}

						float NearClipPlane = camera.Camera.GetOrthographicNearClip();
						if (ImGui::DragFloat("Near Plane", &NearClipPlane, 0.1f))
						{
							camera.Camera.SetOrthographicNearClip(NearClipPlane);
						}

						float FarClipPlane = camera.Camera.GetOrthographicFarClip();
						if (ImGui::DragFloat("Far Plane", &FarClipPlane, 0.1f))
						{
							camera.Camera.SetOrthographicFarClip(FarClipPlane);
						}
					}
					ImGui::TreePop();
				}
				if (remove == true)
				{
					m_Selection.RemoveComponent<CameraComponent>();
				}

			}
			if (m_Selection.HasComponent<AudioSourceComponent>())
			{
				AudioSourceComponent& audiosourcecomp = m_Selection.GetComponent<AudioSourceComponent>();
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				bool open = ImGui::TreeNodeEx((void*)(std::hash<std::string>{}("AudioSource")), treeNodeFlags, "AudioSource");
				ImGui::PopStyleVar();
				bool remove = false;
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						remove = true;
					}
					ImGui::EndPopup();
				}
				if (open)
				{
				
					ImGui::Button("Audio Clip", { ImGui::GetContentRegionAvail().x,20 });
					if (ImGui::BeginDragDropTarget())
					{

						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path audioPath = root / path;
							if (audioPath.extension() == ".mp3" || audioPath.extension() == ".ogg")
							{
								Asset asset = Application::Get().assetManager->GetAssetFromPath<Ref<AudioSource>>(audioPath.string());
								audiosourcecomp.AudioAsset = asset;
							}

						}
						ImGui::EndDragDropTarget();
					}
					ImGui::Checkbox("Play On Awake", &audiosourcecomp.PlayOnAwake);
					ImGui::Checkbox("Spatial", &audiosourcecomp.Spatial);
					ImGui::Checkbox("Loop", &audiosourcecomp.Loop);
					ImGui::SliderFloat("Gain", &audiosourcecomp.Gain,0.0f,1.0f);
					ImGui::SliderFloat("Pitch", &audiosourcecomp.Pitch, 0.0f, 1.0f);
					ImGui::TreePop();
				}
				if (remove == true)
				{
					m_Selection.RemoveComponent<AudioSourceComponent>();
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
				
				if (!m_Selection.HasComponent<Rigidbody2DComponent>())
				{
					if (ImGui::MenuItem("RigidBody 2D"))
					{
						m_Selection.AddComponent<Rigidbody2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_Selection.HasComponent<BoxCollider2DComponent>())
				{
					if (ImGui::MenuItem("Box Collider 2D"))
					{
						m_Selection.AddComponent<BoxCollider2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_Selection.HasComponent<AudioSourceComponent>())
				{
					if (ImGui::MenuItem("Audio Source"))
					{
						m_Selection.AddComponent<AudioSourceComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
			
		}
		ImGui::End();
	}
}

