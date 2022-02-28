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

	bool MonoServer::HasComponent_Interal(int entityID, MonoObject* type)
	{
		Entity entity((entt::entity)entityID, Application::GetInstance()._sceneManager->GetActiveScene().get());
		MonoClass* klass = mono_object_get_class(type);
		MonoString* monostring = (MonoString*)mono_property_get_value(mono_class_get_property_from_name(klass, "Name"), type, nullptr, nullptr);
		const char* name = mono_string_to_utf8(monostring);
			if (strcmp(name,"Transform"))
			{
				return entity.HasComponent<TransformComponent>();
			}
			if (strcmp(name, "NameComponent"))
			{
				return entity.HasComponent<NameComponent>();
			}
		return false;
	}
	void MonoServer::SetPosition_Internal(int entityID, glm::vec3* vector)
	{
		Entity entity((entt::entity)(uint32_t)entityID, Application::GetInstance()._sceneManager->GetActiveScene().get());
		entity.GetComponent<TransformComponent>().Position = *vector;
	}

	void MonoServer::GetPosition_Internal(int entityID, glm::vec3* vector)
	{
		Entity entity((entt::entity)entityID, Application::GetInstance()._sceneManager->GetActiveScene().get());
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

	void MonoServer::Bindinternals()
	{

		mono_add_internal_call("Lithium.Core.Debug::Log", MonoServer::Log);
		mono_add_internal_call("Lithium.Core.Entity::HasComponent_Internal", MonoServer::HasComponent_Interal);
		mono_add_internal_call("Lithium.Core.Transform::SetPosition_Internal", MonoServer::SetPosition_Internal);
		mono_add_internal_call("Lithium.Core.Transform::GetPosition_Internal", MonoServer::GetPosition_Internal);
		mono_add_internal_call("Lithium.Core.Input::MouseKeyPressed", MonoServer::MouseKey_Internal);
		mono_add_internal_call("Lithium.Core.Input::IsKeyPressed", MonoServer::KeyPressed_Internal);
		mono_add_internal_call("Lithium.Core.Input::MousePosition_Internal", MonoServer::MousePosition_Internal);
	}

	void MonoServer::LoadAllClassesInImage()
	{
		_AllClassesInImage.clear();
		MonoClass* klass;
		int i;

		int rows = mono_image_get_table_rows(_MonoImage, MONO_TABLE_TYPEDEF);

		/* we start the count from 1 because we skip the special type <Module> */
		for (i = 1; i < rows; ++i) {
			klass = mono_class_get(_MonoImage, (i + 1) | MONO_TOKEN_TYPE_DEF);
			std::string _Nspace = mono_class_get_namespace(klass);
			std::string name = mono_class_get_name(klass);
			_AllClassesInImage.emplace(name, name + "." + _Nspace);
		}
	}

	MonoServer::MonoServer()
	{
		lastassemblytime = std::filesystem::last_write_time(_Path);

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
		_ClassCache.clear();
		_ScriptBaseClass = mono_class_from_name(_MonoImage, "Lithium.Core", "Script");

		
	}
	void MonoServer::DeleteAssemblies()
	{
			mono_domain_set(_MonoRootDomain, 0);
			mono_domain_unload(_MonoAppDomain);
	}
	bool MonoServer::CheckForChange()
	{

		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		if (std::filesystem::last_write_time(_Path) != lastassemblytime)
		{
			lastassemblytime = std::filesystem::last_write_time(_Path);
			
			DeleteAssemblies();
			std::filesystem::copy_file(_Path.c_str(), _BinPath.c_str(), std::filesystem::copy_options::overwrite_existing);

			Reload();
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
			//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
			CORE_LOG("updated assembly");
			return true;
		}
		else
		{
			return false;
		}


	}

	bool MonoServer::CheckIfClassExists(const std::string& name)
	{
		return (_AllClassesInImage.find(name) == _AllClassesInImage.end());
	}

	void MonoServer::SetActiveScene(const Ref<Scene>& scene)
	{
		m_ActiveScene = scene;
	}

	Ref<ScriptClass> MonoServer::GetClass(const std::string& name)
	{

		
		if (_ClassCache.find(name) != _ClassCache.end())
		{
			return _ClassCache[name];
		}
		else
		{
			MonoClass* klass = mono_class_from_name(_MonoImage, "", name.c_str());
			
			if (klass == NULL)
			{
				CORE_LOG("class not found : '" << name << "'");
				return nullptr;
			}

			Ref<ScriptClass> _scriptclass = CreateRef<ScriptClass>(klass,_MonoAppDomain);
			if (mono_class_is_subclass_of(klass, _ScriptBaseClass, false))
			{
				_scriptclass->IsSubClassFromScript = true;
			}
			_scriptclass->SetScriptBaseClass(_ScriptBaseClass);
			_scriptclass->SetName(name);
			_scriptclass->LoadFields();
			_ClassCache.emplace(name,_scriptclass);
		
			return _ClassCache[name];
		}
	}


}
