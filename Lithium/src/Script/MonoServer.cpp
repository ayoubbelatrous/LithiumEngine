#include "lipch.h"

#include "Script/MonoServer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Core/Application.h"
#include "Scene/Components.h"
#include "Input/Input.h"
#include <chrono>
#include <assert.h>
#include "gtc/type_ptr.hpp"

#include <mono/metadata/tokentype.h>

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
		std::unordered_map<UUID, entt::entity> enttMap;

		auto& registry = Application::Get().sceneManager->GetActiveScene()->GetRegistry();
		auto entities = registry.view<IDComponent>();
		
		for (auto entity : entities)
		{
			
			IDComponent idc = registry.get<IDComponent>(entity);
			enttMap.emplace(idc.ID,entity);
		}
		UUID uuid = UUID(entityID);
		Entity entity(enttMap[uuid], Application::Get().sceneManager->GetActiveScene().get());


		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);

		if (strcmp(name, "Transform"))
		{
			return entity.HasComponent<TransformComponent>();

		}
		else if (strcmp(name, "NameComponent"))
		{
			return entity.HasComponent<NameComponent>();

		}
		return false;
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

	bool MonoServer::MouseKey_Internal(int button)
	{
		return Input::IsMouseKeyPressed(button);
	}

	void MonoServer::MousePosition_Internal(glm::vec2* pos)
	{
		memcpy(pos, &Input::MousePosition(), sizeof(glm::vec2));
	}

	bool MonoServer::KeyPressed_Internal(uint16_t button)
	{
		return Input::IsKeyPressed((KeyCode)button);
	}

	double MonoServer::DeltaTime_Internal()
	{
		return Application::Get().GetDeltaTime();
	}

	void MonoServer::ForwardMonoException(MonoObject* object)
	{
		void* Args[1];
		Args[0] = object;
		mono_runtime_invoke(m_ExceptionMethod, nullptr,Args, nullptr);
	}

	void MonoServer::Bindinternals()
	{

		mono_add_internal_call("Lithium.Core.Debug::Log", MonoServer::Log);
		mono_add_internal_call("Lithium.Core.Entity::HasComponent_Internal", MonoServer::HasComponent_Interal);
		mono_add_internal_call("Lithium.Core.Transform::SetPosition_Internal", MonoServer::SetPosition_Internal);
		mono_add_internal_call("Lithium.Core.Transform::GetPosition_Internal", MonoServer::GetPosition_Internal);
		mono_add_internal_call("Lithium.Core.Input::MouseKeyPressed", MonoServer::MouseKey_Internal);
		mono_add_internal_call("Lithium.Core.Input::IsKeyPressed", MonoServer::KeyPressed_Internal);
		mono_add_internal_call("Lithium.Core.Input::MousePosition_Internal", MonoServer::MousePosition_Internal);
		mono_add_internal_call("Lithium.Core.Time::DeltaTime_Internal", MonoServer::DeltaTime_Internal);
	}

	void MonoServer::LoadAllClassesInImage()
	{
		m_AllClassesInImage.clear();
		MonoClass* klass;
		int i;

		int rows = mono_image_get_table_rows(_MonoImage, MONO_TABLE_TYPEDEF);

		/* we start the count from 1 because we skip the special type <Module> */
		for (i = 1; i < rows; ++i) {
			klass = mono_class_get(_MonoImage, (i + 1) | MONO_TOKEN_TYPE_DEF);
			std::string _Nspace = mono_class_get_namespace(klass);
			std::string name = mono_class_get_name(klass);
			m_AllClassesInImage.emplace(name, name + "." + _Nspace);
		}
	}

	MonoServer::MonoServer()
	{
		m_LastAssemblyTime = std::filesystem::last_write_time(_Path);

		InitMono();
		Bindinternals();
	}
	MonoServer::~MonoServer()
	{
		mono_jit_cleanup(_MonoRootDomain);
	}

	

	void MonoServer::InitMono()
	{
	
		std::filesystem::copy_file(_Path.c_str(),_BinPath.c_str(), std::filesystem::copy_options::overwrite_existing);

		mono_set_dirs("assets/CsharpAssemblies/", ".");

		_MonoRootDomain = mono_jit_init("RootDomain");
		_MonoAppDomain = mono_domain_create_appdomain("CsharpAssembly", NULL);

		//creates app domain and sets as the target domain
		if (_MonoAppDomain)
		{
			mono_domain_set(_MonoAppDomain, 0);
		}
		else
		{
			CORE_LOG("[CS MONO] : failed to create appdomain ' _MonoAppDomain '  ");
			
			assert("");
		}

		
		_MonoAssembly = mono_domain_assembly_open(mono_domain_get(),_BinPath.c_str());
		if (_MonoAssembly)
		{
			_MonoImage = mono_assembly_get_image(_MonoAssembly);

		}
		else
		{
			CORE_LOG("[CS MONO] : failed to load assembly ' _MonoAssembly '  ");

			assert("");
		}


		if (!_MonoImage)
		{
			CORE_LOG("[CS MONO] : failed to create assembly ' _MonoImage '  ");

			assert("");
		}
		_ScriptBaseClass =  mono_class_from_name(_MonoImage, "Lithium.Core", "Script");
		MonoMethodDesc* excDesc = mono_method_desc_new("Lithium.Core.Debug::OnException(object)", true);
		m_ExceptionMethod = mono_method_desc_search_in_image(excDesc, _MonoImage);
		LoadAllClassesInImage();
	}


	void MonoServer::Reload()
	{
		_MonoAppDomain = mono_domain_create_appdomain("CsharpAssembly", NULL);
		if (_MonoAppDomain)
		mono_domain_set(_MonoAppDomain, 0);
		else
		{
			CORE_LOG("error domain");
		}

		_MonoAssembly = mono_domain_assembly_open(mono_domain_get(), _BinPath.c_str());
		_MonoImage = mono_assembly_get_image(_MonoAssembly);
		Bindinternals();
		_ScriptBaseClass = mono_class_from_name(_MonoImage, "Lithium.Core", "Script");
		MonoMethodDesc* excDesc = mono_method_desc_new("Lithium.Core.Debug::OnException", true);
		m_ExceptionMethod = mono_method_desc_search_in_image(excDesc, _MonoImage);
		m_ScriptClassMap.clear();
		LoadAllClassesInImage();
	}
	void MonoServer::DeleteAssemblies()
	{
			mono_domain_set(_MonoRootDomain, 0);
			mono_domain_unload(_MonoAppDomain);
	}
	bool MonoServer::CheckForChange()
	{

		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		if (std::filesystem::last_write_time(_Path) != m_LastAssemblyTime)
		{
			m_LastAssemblyTime = std::filesystem::last_write_time(_Path);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
			//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
			return true;
		}
		else
		{
			return false;
		}


	}

	bool MonoServer::CheckIfClassExists(const std::string& name)
	{
		return (m_AllClassesInImage.find(name) == m_AllClassesInImage.end());
	}

	void MonoServer::ForceReload()
	{
		DeleteAssemblies();
		std::filesystem::copy_file(_Path.c_str(), _BinPath.c_str(), std::filesystem::copy_options::overwrite_existing);
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
			monoklass = mono_class_from_name(_MonoImage, "", name.c_str());
			if (monoklass == nullptr)
			{
				CORE_LOG("mono class not found");
				return scriptObject;
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


		MonoObject* monoobject = mono_object_new(_MonoAppDomain, scriptClass->GetClassPtr());
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

	void* MonoServer::CreateMonoString(const char* str)
	{
		return mono_string_new(_MonoAppDomain, str);
	}

}
