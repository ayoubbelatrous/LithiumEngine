#include "lipch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include <mono/metadata/debug-helpers.h>
#include "Core/Base.h"
#include "MonoServer.h"
#include <assert.h>

#include <chrono>
#include "Script/ScriptError.h"
#include <mono/metadata/environment.h>


namespace Lithium
{
	static void Log(MonoString* log) {
		const char* text = mono_string_to_utf8(log);
		CORE_LOG(text);
	}
	static void BindFunc(const std::string& name,const void* Func)
	{
		mono_add_internal_call(name.c_str(), Func);
	}
	void MonoServer::Bindinternals()
	{
		//Binds debug class
		BindFunc("Debug:Log", Log);
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
			CORE_LOG("updating assembly");
			DeleteAssemblies();
			std::filesystem::copy_file(_Path.c_str(), _BinPath.c_str(), std::filesystem::copy_options::overwrite_existing);

			Reload();
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
				SCRIPT_ERROR("class not found : '" << name << "'");
				return nullptr;
			}
			Ref<ScriptClass> _scriptclass = CreateRef<ScriptClass>((uint32_t)klass);
			_scriptclass->SetName(name);
			_scriptclass->LoadFields();
			_ClassCache.emplace(name,_scriptclass);
		
			return _ClassCache[name];
		}
	}


}
