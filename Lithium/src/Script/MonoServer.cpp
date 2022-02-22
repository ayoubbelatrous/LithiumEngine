#include "lipch.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include <mono/metadata/debug-helpers.h>
#include "Core/Base.h"
#include "MonoServer.h"
#include <cassert>


namespace Lithium
{
	/*bool mono_destory_domain_with_assisisated_assembly(MonoDomain* domain_to_destroy) {
		if (domain_to_destroy != _MainDomain) {
			mono_domain_set(_MainDomain, 0);
			mono_domain_unload(domain_to_destroy);
			return true;
		}
		return false;
	}*/
	void Test()
	{
		












		//mono_set_dirs("assets/CsharpAssemblies/", "assets/CsharpAssemblies/");

		//_MainDomain = mono_jit_init_version("Root", "v4.0.30319");
		//_Domain = mono_domain_create_appdomain("Csharp", NULL);
		//if (_Domain)
		//	mono_domain_set(_Domain, 0);

		//	// Open assembly.
		//	_Assembly = mono_domain_assembly_open(mono_domain_get(), "assets/TestProject/Assem/Csharp.dll");
		//	if (_Assembly) 
		//		_Image = mono_assembly_get_image(_Assembly);
		//	
		//mono_add_internal_call("Debug::Log", Log);
	
		//MonoClass* klass = mono_class_from_name(_Image, "", "Test");
		//if (!klass)
		//	CORE_LOG("!!!Loaded class TEST");

		//MonoObject* object = mono_object_new(_Domain, klass);


		//MonoClass* klass2 = mono_class_from_name(_Image, "", "Hello");
		//if (!klass)
		//	CORE_LOG("!!!Loaded class HELLo");

		//MonoObject* object2 = mono_object_new(_Domain, klass2);
	
		//mono_runtime_object_init(object);
		//mono_runtime_object_init(object2);
		//if (!object)
		//	CORE_LOG("!!!created object class");

		//if (!object2)
		//	CORE_LOG("!!!created object2 class");

		//std::vector<std::string> _FieldsNames;
		//std::vector<MonoClassField*> _Fields;
		//MonoClassField* field;
		//void* iter = nullptr;

		//while ((field = mono_class_get_fields(klass, &iter)) != nullptr)
		//{
		//	if ((mono_field_get_flags(field)) == 0)
		//		continue;
		//	_FieldsNames.push_back(mono_field_get_name(field));
		//	_Fields.push_back(field);
		//}

		//

		//mono_field_set_value(object, _Fields[0],object2);


		//
		//MonoMethodDesc* BarkMethodDesc;
		//const char* BarkMethodDescStr = "Test:Update";
		//BarkMethodDesc = mono_method_desc_new(BarkMethodDescStr, NULL);
		//if (!BarkMethodDesc)
		//	CORE_LOG("!!!desc success");

		////Search the method in the image
		//MonoMethod* method;
		//method = mono_method_desc_search_in_image(BarkMethodDesc, _Image);
		//if (!method)
		//	CORE_LOG("!!!method update in test found");
		//void* args[1];
		//float delta = 0.0116f;
		//args[0] = &delta;

		////Run the method
		//std::cout << "Running the method: " << BarkMethodDescStr << std::endl;

		//for (size_t i = 0; i < 1; i++)
		//{
		//	mono_runtime_invoke(method, object, args, nullptr);

		//}


		//if (std::cin.get()) {
		//	mono_destory_domain_with_assisisated_assembly(_Domain);

		//	if (std::cin.get())
		//	{
		//		CORE_LOG("update assembly!");
		//		//update assembly
		//	}
		//	_Domain = mono_domain_create_appdomain("Csharp", NULL);
		//	if (_Domain)
		//		mono_domain_set(_Domain, 0);

	
		//	mono_image_close(_Image);
		//	
		//	_Assembly = mono_domain_assembly_open(mono_domain_get(), "assets/TestProject/Assem/Csharp.dll");
		//	if (_Assembly)
		//		_Image = mono_assembly_get_image(_Assembly);
		//}


		//mono_add_internal_call("Debug::Log", Log);
		//MonoClass* testcla = mono_class_from_name(_Image, "", "Test");
		//if (!klass)
		//	CORE_LOG("!!!Loaded class TEST2");

		//MonoObject* objj = mono_object_new(_Domain, testcla);

		//mono_runtime_object_init(objj);

		
	}
	MonoServer::MonoServer()
	{
		InitMono();
	}
	MonoServer::~MonoServer()
	{
		mono_jit_cleanup(_MonoRootDomain);
	}
	void MonoServer::InitMono()
	{
		mono_set_dirs("assets/CsharpAssemblies/", "assets/CsharpAssemblies/");

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
			
			assert("[CS MONO] : failed to create appdomain ' _MonoAppDomain '");
		}

		
		_MonoAssembly = mono_domain_assembly_open(mono_domain_get(),_Path.c_str());
		if (_MonoAssembly)
		{
			_MonoImage = mono_assembly_get_image(_MonoAssembly);

		}
		else
		{
			CORE_LOG("[CS MONO] : failed to load assembly ' _MonoAssembly '  ");

			assert("[CS MONO] : failed to load assembly ' _MonoAssembly '  ");
		}


		if (!_MonoImage)
		{
			CORE_LOG("[CS MONO] : failed to create assembly ' _MonoImage '  ");

			assert("[CS MONO] : failed to create assembly ' _MonoImage '  ");
		}
		
		
	}
}
