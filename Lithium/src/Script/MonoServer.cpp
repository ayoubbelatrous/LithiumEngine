#include "lipch.h"

#include "Script/MonoServer.h"


#include <chrono>
#include <assert.h>




namespace Lithium
{
	std::vector<const char*> MonoServer::_BufferLog = std::vector<const char*>();

	void MonoServer::Log(MonoString* log)
	{
		const char* text = mono_string_to_utf8(log);
		_BufferLog.push_back(text);
	}
	void MonoServer::Bindinternals()
	{
		
		mono_add_internal_call("Debug::Log", MonoServer::Log);
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
		_ComponentBaseClass =  mono_class_from_name(_MonoImage, "", "Component");
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
		_ComponentBaseClass = mono_class_from_name(_MonoImage, "", "Component");

		

		
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
			if (mono_class_is_subclass_of(klass, _ComponentBaseClass, false))
			{
				_scriptclass->IsSubClassFromComponent = true;
			}
			_scriptclass->SetComponentClass(_ComponentBaseClass);
			_scriptclass->SetName(name);
			_scriptclass->LoadFields();
			_ClassCache.emplace(name,_scriptclass);
		
			return _ClassCache[name];
		}
	}


}
