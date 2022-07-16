#include "lipch.h"

#include "Script/MonoServer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Core/Application.h"
#include "Scene/Components.h"
#include "Input/Input.h"
#include <chrono>
#include <assert.h>
#include "glm/gtc/type_ptr.hpp"

#include <mono/metadata/tokentype.h>
#include <mono/metadata/mono-gc.h>
#include "Audio/Audio.h"

namespace Lithium
{
	std::vector<const char*> MonoServer::_BufferLog = std::vector<const char*>();


	void MonoServer::Log(MonoString* log)
	{
		const char* text = mono_string_to_utf8(log);
		_BufferLog.push_back(text);
	}

	bool MonoServer::HasComponent_Interal(uint64_t entityID, MonoObject* type)
	{
		
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());


		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);

		if (strcmp(name, "Transform") == 0)
		{
			return entity.HasComponent<TransformComponent>();
		}
		else if (strcmp(name, "NameComponent") == 0)
		{
			return entity.HasComponent<NameComponent>();
		}
		else if (strcmp(name, "SpriteRenderer") == 0)
		{
			return entity.HasComponent<SpriteRendererComponent>();
		}
		else if (strcmp(name, "Rigidbody2D") == 0)
		{
			return entity.HasComponent<Rigidbody2DComponent>();
		}
		else if (strcmp(name, "Camera") == 0)
		{
			return entity.HasComponent<CameraComponent>();
		}
		else if (strcmp(name, "AudioSource") == 0)
		{
			return entity.HasComponent<AudioSourceComponent>();
		}
		else if (strcmp(name, "TextRenderer") == 0)
		{
			return entity.HasComponent<TextRenderer>();
		}
		return false;
	}

	bool MonoServer::AddComponent_Interal(uint64_t entityID, MonoObject* type)
	{
		
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());

		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);

		if (strcmp(name, "Transform") == 0)
		{
			if (entity.HasComponent<TransformComponent>())
			{
				return false;
			}
			else
			{
				entity.AddComponent<TransformComponent>();
			}
		}
		else if (strcmp(name, "NameComponent") == 0)
		{
			if (entity.HasComponent<NameComponent>())
			{
				return false;
			}
			else
			{
				entity.AddComponent<NameComponent>();
			}
		}
		else if (strcmp(name, "SpriteRenderer") == 0)
		{
			if (entity.HasComponent<SpriteRendererComponent>())
			{
				return false;
			}
			else
			{
				entity.AddComponent<SpriteRendererComponent>();
			}
		}
		return false;
	}

	bool MonoServer::HasScript_Interal(uint64_t entityID, MonoObject* type)
	{

		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());

		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);

		for (auto script : entity.GetComponent<ScriptGroupeComponent>().Scripts)
		{
			if (strcmp(name, script.Name.c_str()) == 0)
			{
				return true;

			}
		}
		return false;
	}

	bool MonoServer::AddScript_Interal(uint64_t entityID, MonoObject* type)
	{
		return false;
	}

	MonoObject* MonoServer::GetScript_Internal(uint64_t entityID, MonoObject* type)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());

		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);

		for (auto script : entity.GetComponent<ScriptGroupeComponent>().Scripts)
		{
			if (strcmp(name, script.Name.c_str()) == 0)
			{
				return (MonoObject*)script.Scriptobject->GetObjectPtr();

			}
		}

		return nullptr;
	}

	uint64_t MonoServer::CopyEntity_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		Entity dupedEntity = Application::Get().sceneManager->GetActiveScene()->DuplicateEntity(entity);
		return dupedEntity.GetComponent<IDComponent>().ID;
	}

	void MonoServer::SetPosition_Internal(uint64_t entityID, glm::vec3* vector)
	{
		
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<TransformComponent>().Position = *vector;
	}

	void MonoServer::GetPosition_Internal(uint64_t entityID, glm::vec3* vector)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TransformComponent& trans = entity.GetComponent<TransformComponent>();
		memcpy(vector, &trans.Position,sizeof(glm::vec3));
	}

	void MonoServer::SetRotation_Internal(uint64_t entityID, glm::vec3* vector)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<TransformComponent>().Rotation = *vector;
	}

	void MonoServer::GetRotation_Internal(uint64_t entityID, glm::vec3* vector)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TransformComponent& trans = entity.GetComponent<TransformComponent>();
		memcpy(vector, &trans.Rotation, sizeof(glm::vec3));
	}

	void MonoServer::SetScale_Internal(uint64_t entityID, glm::vec3* vector)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<TransformComponent>().Scale = *vector;
	}

	void MonoServer::GetScale_Internal(uint64_t entityID, glm::vec3* vector)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TransformComponent& trans = entity.GetComponent<TransformComponent>();
		memcpy(vector, &trans.Scale, sizeof(glm::vec3));
	}

	void MonoServer::SetColor_Internal(uint64_t entityID, glm::vec4* color)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	
	}

	void MonoServer::GetColor_Internal(uint64_t entityID, glm::vec4* color)
	{

		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		SpriteRendererComponent& renderer = entity.GetComponent<SpriteRendererComponent>();
		memcpy(color, &renderer.Color, sizeof(glm::vec4));
	}

	void MonoServer::SetRigidbodyFixedRotation_Internal(uint64_t entityID, bool* fixedRotation)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		rb2d.FixedRotation = *fixedRotation;
	}

	bool MonoServer::GetRigidbodyFixedRotation_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		return rb2d.FixedRotation;
	}

	void MonoServer::SetRigidbodyVelocity_Internal(uint64_t entityID, glm::vec2* velocity)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<Rigidbody2DComponent>().SetVelocity(*velocity);
	}

	void MonoServer::GetRigidbodyVelocity_Internal(uint64_t entityID, glm::vec2* velocity)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		Rigidbody2DComponent& b2bd = entity.GetComponent<Rigidbody2DComponent>();
		glm::vec2 b2velocity = b2bd.GetVelocity();
		memcpy(velocity, &b2velocity, sizeof(glm::vec2));
	}

	void MonoServer::SetCameraProjection_Internal(uint64_t entityID, int* projection)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType((SceneCamera::ProjectionType)*projection);
	}

	int MonoServer::GetCameraProjection_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return (int)entity.GetComponent<CameraComponent>().Camera.GetProjectionType();
	}

	void MonoServer::SetCameraOrthographicSize_Internal(uint64_t entityID, float* size)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(*size);

	}

	float MonoServer::GetCameraOrthographicSize_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicSize();

	}

	void MonoServer::SetCameraPrimary_Internal(uint64_t entityID, bool* primary)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<CameraComponent>().Primary = *primary;
	}

	bool MonoServer::GetCameraPrimary_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return entity.GetComponent<CameraComponent>().Primary;
	}

	void MonoServer::SetCameraFixedAspectRatio_Internal(uint64_t entityID, bool* fixedaspectration)
	{

		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		entity.GetComponent<CameraComponent>().FixedAspectRatio = *fixedaspectration;
	}

	bool MonoServer::GetCameraFixedAspectRatio_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return entity.GetComponent<CameraComponent>().FixedAspectRatio;
	}

	void MonoServer::SetAudioSourcePlay_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		AudioSourceComponent& audiosource = entity.GetComponent<AudioSourceComponent>();
		if (audiosource.AudioAsset.GetUUID() == 0)
		{
			return;
		}
		Ref<AudioSource> source = Application::Get().assetManager->GetAsset<Ref<AudioSource>>(audiosource.AudioAsset);
		Audio::Play(source);
		
	}

	void MonoServer::SetAudioSourceLoop_Internal(uint64_t entityID, bool loop)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		AudioSourceComponent& audiosource = entity.GetComponent<AudioSourceComponent>();
		audiosource.Loop = loop;
	}

	bool MonoServer::GetAudioSourceLoop_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return entity.GetComponent<AudioSourceComponent>().Loop;
	}

	void MonoServer::SetAudioSourceGain_Internal(uint64_t entityID, float gain)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		AudioSourceComponent& audiosource = entity.GetComponent<AudioSourceComponent>();
		audiosource.Gain = gain;
	}

	float MonoServer::GetAudioSourceGain_Internal(uint64_t entityID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		return entity.GetComponent<AudioSourceComponent>().Gain;
	}

	void MonoServer::AudioSourcePlayClip_Internal(uint64_t entityID, uint64_t assetID)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		if (assetID == 0)
		{
			return;
		}
		Ref<AudioSource> audioclip  = Application::Get().assetManager->GetAsset<Ref<AudioSource>>(Asset(assetID));
		audioclip->SetPosition(glm::value_ptr(transform.Position));
		Audio::Play(audioclip);
	}

	void MonoServer::SetTextRendererText_Internal(uint64_t entityID, MonoString* text)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TextRenderer& textrenderer = entity.GetComponent<TextRenderer>();
		textrenderer.Text = mono_string_to_utf8(text);
	}

	void MonoServer::GetTextRendererText_Internal(uint64_t entityID, MonoString** outText)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		TextRenderer& textrenderer = entity.GetComponent<TextRenderer>();
		*outText = (MonoString*)Application::Get().Monoserver->CreateMonoString(textrenderer.Text.c_str());
	}

	MonoString* MonoServer::GetName_Internal(uint64_t entityID)
	{
		
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		NameComponent& namecomp = entity.GetComponent<NameComponent>();
		return (MonoString*)Application::Get().Monoserver->CreateMonoString(namecomp.GetName().c_str());
	}

	void MonoServer::SetName_Internal(uint64_t entityID, MonoString* name)
	{
		Entity entity(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[entityID], Application::Get().sceneManager->GetActiveScene().get());
		NameComponent& namecomp = entity.GetComponent<NameComponent>();
		namecomp.Name = mono_string_to_utf8(name);
	}

	bool MonoServer::MouseKey_Internal(int button)
	{
		return Input::IsMouseKeyPressed(button);
	}

	bool MonoServer::MouseKeyDown_Internal(int button)
	{
		return Input::IsMouseKeyDown(button);

	}

	void MonoServer::MousePosition_Internal(glm::vec2* pos)
	{
		glm::vec2 MousePosition = Input::MousePosition();
		memcpy(pos,&MousePosition , sizeof(glm::vec2));
	}

	bool MonoServer::KeyPressed_Internal(uint16_t button)
	{
		return Input::IsKeyPressed((KeyCode)button);
	}

	bool MonoServer::KeyPressedDown_Internal(uint16_t button)
	{
		return Input::IsKeyDown((KeyCode)button);

	}

	double MonoServer::DeltaTime_Internal()
	{
		return Application::Get().GetDeltaTime();
	}

	uint32_t MonoServer::CreateTexture_Internal(int width, int height, int format)
	{
		return Application::Get().assetManager->CreateUserTexture(width, height, format);
	}

	void MonoServer::DeleteTexture_Internal(int id)
	{
		Application::Get().assetManager->DeleteUserTexture(id);
	}

	void MonoServer::SetTextureData_Internal(uint32_t id, glm::vec4* data,int size)
	{
		Application::Get().assetManager->SetUserTextureData(id, data);
	}

	void MonoServer::ForwardMonoException(MonoObject* object)
	{
		void* Args[1];
		Args[0] = object;
		mono_runtime_invoke(m_ExceptionMethod, nullptr,Args, nullptr);
	}

	void MonoServer::Bindinternals()
	{
		mono_add_internal_call("Lithium.Core.Debug::Log", (const void*)MonoServer::Log);
		mono_add_internal_call("Lithium.Core.Entity::HasComponent_Internal", (const void*)MonoServer::HasComponent_Interal);
		mono_add_internal_call("Lithium.Core.Entity::AddComponent_Internal", (const void*)MonoServer::AddComponent_Interal);

		mono_add_internal_call("Lithium.Core.Entity::HasScript_Internal", (const void*)MonoServer::HasScript_Interal);
		mono_add_internal_call("Lithium.Core.Entity::GetScript_Internal", (const void*)MonoServer::GetScript_Internal);

		mono_add_internal_call("Lithium.Core.Script::CopyEntity_Internal", (const void*)MonoServer::CopyEntity_Internal);

		mono_add_internal_call("Lithium.Core.Transform::SetPosition_Internal", (const void*)MonoServer::SetPosition_Internal);
		mono_add_internal_call("Lithium.Core.Transform::GetPosition_Internal", (const void*)MonoServer::GetPosition_Internal);

		mono_add_internal_call("Lithium.Core.Transform::SetRotation_Internal", (const void*)MonoServer::SetRotation_Internal);
		mono_add_internal_call("Lithium.Core.Transform::GetRotation_Internal", (const void*)MonoServer::GetRotation_Internal);

		mono_add_internal_call("Lithium.Core.Transform::SetScale_Internal", (const void*)MonoServer::SetScale_Internal);
		mono_add_internal_call("Lithium.Core.Transform::GetScale_Internal", (const void*)MonoServer::GetScale_Internal);

		mono_add_internal_call("Lithium.Core.NameComponent::SetName_Internal", (const void*)MonoServer::SetName_Internal);
		mono_add_internal_call("Lithium.Core.NameComponent::GetName_Internal", (const void*)MonoServer::GetName_Internal);

		mono_add_internal_call("Lithium.Core.SpriteRenderer::SetColor_Internal", (const void*)MonoServer::SetColor_Internal);
		mono_add_internal_call("Lithium.Core.SpriteRenderer::GetColor_Internal",(const void*) MonoServer::GetColor_Internal);

		mono_add_internal_call("Lithium.Core.Rigidbody2D::SetRigidbodyFixedRotation_Internal", (const void*)MonoServer::SetRigidbodyFixedRotation_Internal);
		mono_add_internal_call("Lithium.Core.Rigidbody2D::GetRigidbodyFixedRotation_Internal",(const void*) MonoServer::GetRigidbodyFixedRotation_Internal);

		mono_add_internal_call("Lithium.Core.Rigidbody2D::SetRigidbodyVelocity_Internal", (const void*)MonoServer::SetRigidbodyVelocity_Internal);
		mono_add_internal_call("Lithium.Core.Rigidbody2D::GetRigidbodyVelocity_Internal", (const void*)MonoServer::GetRigidbodyVelocity_Internal);

		mono_add_internal_call("Lithium.Core.Camera::SetCameraProjection_Internal", (const void*)MonoServer::SetCameraProjection_Internal);
		mono_add_internal_call("Lithium.Core.Camera::GetCameraProjection_Internal", (const void*)MonoServer::GetCameraProjection_Internal);

		mono_add_internal_call("Lithium.Core.Camera::SetCameraPrimary_Internal", (const void*)MonoServer::SetCameraPrimary_Internal);
		mono_add_internal_call("Lithium.Core.Camera::GetCameraPrimary_Internal", (const void*)MonoServer::GetCameraPrimary_Internal);

		mono_add_internal_call("Lithium.Core.Camera::SetCameraOrthographicSize_Internal", (const void*)MonoServer::SetCameraOrthographicSize_Internal);
		mono_add_internal_call("Lithium.Core.Camera::GetCameraOrthographicSize_Internal", (const void*)MonoServer::GetCameraOrthographicSize_Internal);

		mono_add_internal_call("Lithium.Core.Camera::SetCameraFixedAspectRatio_Internal", (const void*)MonoServer::SetCameraFixedAspectRatio_Internal);
		mono_add_internal_call("Lithium.Core.Camera::GetCameraFixedAspectRatio_Internal", (const void*)MonoServer::GetCameraFixedAspectRatio_Internal);

		mono_add_internal_call("Lithium.Core.AudioSource::SetAudioSourcePlay_Internal", (const void*)MonoServer::SetAudioSourcePlay_Internal);

		mono_add_internal_call("Lithium.Core.AudioSource::SetAudioSourceLoop_Internal", (const void*)MonoServer::SetAudioSourceLoop_Internal);
		mono_add_internal_call("Lithium.Core.AudioSource::GetAudioSourcePlay_Internal", (const void*)MonoServer::GetAudioSourceLoop_Internal);

		mono_add_internal_call("Lithium.Core.AudioSource::SetAudioSourceGain_Internal", (const void*)MonoServer::SetAudioSourceGain_Internal);
		mono_add_internal_call("Lithium.Core.AudioSource::GetAudioSourceGain_Internal", (const void*)MonoServer::GetAudioSourceGain_Internal);

		mono_add_internal_call("Lithium.Core.AudioSource::AudioSourcePlayClip_Internal", (const void*)MonoServer::AudioSourcePlayClip_Internal);

		mono_add_internal_call("Lithium.Core.TextRenderer::SetTextRendererText_Internal", (const void*)MonoServer::SetTextRendererText_Internal);
		mono_add_internal_call("Lithium.Core.TextRenderer::GetTextRendererText_Internal", (const void*)MonoServer::GetTextRendererText_Internal);

		mono_add_internal_call("Lithium.Core.Texture::CreateTexture_Internal", (const void*)MonoServer::CreateTexture_Internal);
		mono_add_internal_call("Lithium.Core.Texture::DeleteTexture_Internal", (const void*)MonoServer::DeleteTexture_Internal);
		mono_add_internal_call("Lithium.Core.Texture::SetTextureData_Internal", (const void*)MonoServer::SetTextureData_Internal);

		mono_add_internal_call("Lithium.Core.Input::MouseKeyPressed", (const void*)MonoServer::MouseKey_Internal);
		mono_add_internal_call("Lithium.Core.Input::MouseKeyDown", (const void*)MonoServer::MouseKeyDown_Internal);
		mono_add_internal_call("Lithium.Core.Input::IsKeyPressed", (const void*)MonoServer::KeyPressed_Internal);
		mono_add_internal_call("Lithium.Core.Input::IsKeyDown", (const void*)MonoServer::KeyPressedDown_Internal);
		mono_add_internal_call("Lithium.Core.Input::MousePosition_Internal", (const void*)MonoServer::MousePosition_Internal);
		mono_add_internal_call("Lithium.Core.Time::DeltaTime_Internal", (const void*)MonoServer::DeltaTime_Internal);
	}

	void MonoServer::LoadAllClassesInImage()
	{
		m_AllClassesInImage.clear();
		MonoClass* klass;
		int i;

		int rows = mono_image_get_table_rows(m_MonoImage, MONO_TABLE_TYPEDEF);

		for (i = 1; i < rows; ++i) {
			klass = mono_class_get(m_MonoImage, (i + 1) | MONO_TOKEN_TYPE_DEF);
			std::string _Nspace = mono_class_get_namespace(klass);
			std::string name = mono_class_get_name(klass);
			MonoClass* BaseClass = nullptr;
			BaseClass = mono_class_get_parent(klass);
			if (BaseClass != nullptr)
			{
				const char* BaseClassName = mono_class_get_name(BaseClass);
				if (strcmp(BaseClassName, "Script") == 0)
				{
					m_AllClassesInImage.emplace(name, _Nspace + "." + name);

				}

			}
			
		}

		
	}

	MonoServer::MonoServer()
	{
		m_LastAssemblyTime = std::filesystem::last_write_time(AssemblyPath);

		InitMono();
		Bindinternals();
	
	}
	MonoServer::~MonoServer()
	{
		mono_jit_cleanup(m_MonoRootDomain);
	}

	

	void MonoServer::InitMono()
	{
		
		mono_set_dirs("Library/assemblies", "."); 

		m_MonoRootDomain = mono_jit_init("RootDomain");
		m_MonoAppDomain = mono_domain_create_appdomain("CsharpAssembly", NULL);

		
		if (m_MonoAppDomain)
		{
			mono_domain_set(m_MonoAppDomain, 0);
		}
		else
		{
			CORE_LOG("[ERROR] : mono domain failed to create!")
		}
		MonoImageOpenStatus OpenStatus;
		uint32_t size = 0;
		char* assemblyData = LoadAssemblyFile(AssemblyPath.c_str(), &size);
		m_MonoImage = mono_image_open_from_data(assemblyData, size, false, &OpenStatus);
		if (m_MonoImage)
		{
			m_MonoAssembly = mono_assembly_load_from(m_MonoImage, "", &OpenStatus);
		}
		else
		{
			CORE_LOG("[ERROR] : mono domain failed to create!")
		}

		m_ScriptBaseClass =  mono_class_from_name(m_MonoImage, "Lithium.Core", "Script");
		MonoMethodDesc* excDesc = mono_method_desc_new("Lithium.Core.Debug::OnException(object)", true);
		m_ExceptionMethod = mono_method_desc_search_in_image(excDesc, m_MonoImage);
		LoadAllClassesInImage();
	}


	void MonoServer::Reload()
	{
		m_MonoAppDomain = mono_domain_create_appdomain("CsharpAssembly", NULL);
		if (m_MonoAppDomain)
		mono_domain_set(m_MonoAppDomain, 0);
		else
		{
			CORE_LOG("error domain");
		}
		MonoImageOpenStatus OpenStatus;
		uint32_t size = 0;
		char* assemblyData = LoadAssemblyFile(AssemblyPath.c_str(), &size);
		m_MonoImage = mono_image_open_from_data(assemblyData, size, false, &OpenStatus);
		m_MonoAssembly = mono_assembly_load_from(m_MonoImage, "", &OpenStatus);


		Bindinternals();


		m_ScriptBaseClass = mono_class_from_name(m_MonoImage, "Lithium.Core", "Script");
		MonoMethodDesc* excDesc = mono_method_desc_new("Lithium.Core.Debug::OnException", true);

		m_ExceptionMethod = mono_method_desc_search_in_image(excDesc, m_MonoImage);
		m_ScriptClassMap.clear();
		
		LoadAllClassesInImage();
	}
	void MonoServer::DeleteAssemblies()
	{
			mono_domain_set(m_MonoRootDomain, 0);
			mono_domain_unload(m_MonoAppDomain);
	}
	bool MonoServer::CheckForChange()
	{

		if (std::filesystem::last_write_time(AssemblyPath) != m_LastAssemblyTime)
		{
			m_LastAssemblyTime = std::filesystem::last_write_time(AssemblyPath);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			return true;
		}
		else
		{
			return false;
		}


	}

	bool MonoServer::CheckIfClassExists(const std::string& name)
	{
		if (m_AllClassesInImage.find(name) == m_AllClassesInImage.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void MonoServer::ForceReload()
	{
		DeleteAssemblies();
		Reload();
	}

	void MonoServer::SetActiveScene(const Ref<Scene>& scene)
	{
		m_ActiveScene = scene;
	}



	Ref<ScriptObject> MonoServer::GetObject(const std::string& name)
	{
		Ref<ScriptClass> scriptClass;
		Ref<ScriptObject> scriptObject;
		//TODO: add class to scriptclass map and search in map
		MonoClass* monoklass;
		if (m_ScriptClassMap.find(name) == m_ScriptClassMap.end())
		{

			monoklass = mono_class_from_name(m_MonoImage, "", name.c_str());
			if (monoklass == nullptr)
			{
				CORE_LOG("mono class not found");
				return nullptr;
			}
			else
			{
				scriptClass = CreateRef<ScriptClass>(monoklass);
				m_ScriptClassMap.emplace(name, scriptClass);
			}
		}
		else
		{
			scriptClass = m_ScriptClassMap[name];
		}


		MonoObject* monoobject = mono_object_new(m_MonoAppDomain, scriptClass->GetClassPtr());
		mono_runtime_object_init(monoobject);
		scriptObject = CreateRef<ScriptObject>(monoobject);
		return scriptObject;
	}
	static void CopyFieldValue(Ref<ScriptField>& Dst, const Ref<ScriptField>& Src)
	{
		ASSERT(Dst->GetType() == Src->GetType());
		switch (Dst->GetType())
		{
		case(ScriptType::Int):
		{
			Dst->SetValue<int>(Src->GetLocalValue<int>());
			break;
		}
		case(ScriptType::Float):
		{
			Dst->SetValue<float>(Src->GetLocalValue<float>());
			break;
		}
		case(ScriptType::Vec2):
		{
			Dst->SetValue<glm::vec2>(Src->GetLocalValue<glm::vec2>());
			break;
		}
		case(ScriptType::Vec3):
		{
			Dst->SetValue<glm::vec3>(Src->GetLocalValue<glm::vec3>());
			break;
		}
		case(ScriptType::Vec4):
		{
			Dst->SetValue<glm::vec4>(Src->GetLocalValue<glm::vec4>());
			break;
		}
		case(ScriptType::String):
		{
			Dst->SetValue<std::string>(Src->GetLocalValue<std::string>());
			break;
		}

		case(ScriptType::Entity):
		{
			Dst->SetValue<uint64_t>(Src->GetLocalValue<uint64_t>());
			break;
		}
		case(ScriptType::AudioClip):
		{
			Dst->SetValue<UUID>(Src->GetLocalValue<UUID>());
			break;
		}
		}
	}
	//creates a new object and copies the fields
	Ref<ScriptObject> MonoServer::CopyObject(const Ref<ScriptObject>& object)
	{

		Ref<ScriptObject> newObject = GetObject(object->GetClassName());

		auto& newFields = newObject->GetFields();
		auto& oldFields = object->GetFields();

		for (auto& field : newObject->GetFields())
		{
			if (oldFields.find(field.first) != oldFields.end())
			{
				CopyFieldValue(field.second, oldFields[field.first]);
			}
		}
		return newObject;
	}

	void* MonoServer::CreateMonoEntity(UUID id)
	{
		MonoClass* EntityClass = mono_class_from_name(m_MonoImage, "Lithium.Core", "Entity");
		MonoObject*  EntityObject = mono_object_new(m_MonoAppDomain, EntityClass);
		mono_runtime_object_init(EntityObject);
		MonoClassField* IDfield = mono_class_get_field_from_name(EntityClass, "ID");
		uint64_t EntityID = (uint64_t)id;
		mono_field_set_value(EntityObject, IDfield, &EntityID);
		return EntityObject;
	}

	void* MonoServer::CreateMonoCollsion2D(UUID EntityID)
	{
		MonoClass* CollisionClass = mono_class_from_name(m_MonoImage, "Lithium.Physics", "Collision2D");
		MonoObject* CollisionObject = mono_object_new(m_MonoAppDomain, CollisionClass);
		mono_runtime_object_init(CollisionObject);

		MonoObject* entity = (MonoObject*)CreateMonoEntity(EntityID);

		MonoClassField* EntityField = mono_class_get_field_from_name(CollisionClass, "entity");
		mono_field_set_value(CollisionObject, EntityField, entity);
		return CollisionObject;
	}

	void* MonoServer::CreateMonoAudioClip(UUID assetID)
	{
		MonoClass* AudioClipClass = mono_class_from_name(m_MonoImage, "Lithium.Core", "AudioClip");
		MonoObject* AudioClipObject = mono_object_new(m_MonoAppDomain, AudioClipClass);
		mono_runtime_object_init(AudioClipObject);
		MonoClassField* AssetIDfield = mono_class_get_field_from_name(AudioClipClass, "AssetId");

		mono_field_set_value(AudioClipObject, AssetIDfield, &assetID);
		return AudioClipObject;
	}

	void* MonoServer::CreateMonoString(const char* str)
	{
		return mono_string_new(m_MonoAppDomain, str);
	}

	char* MonoServer::LoadAssemblyFile(const std::string& path, uint32_t* size)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		*size = file.tellg();
		file.seekg(0, std::ios::beg);
		char* buffer = new char[*size];
		file.read(buffer, *size);
		file.close();
		return buffer;
	}

}
