#pragma once
#include <iostream>
#include "Core/Base.h"
#include "Script/ScriptClassField.h"
#include "Script/ScriptMethod.h"
#include "Script/ScriptProperty.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
namespace Lithium
{

	class ScriptClass;

	class ScriptObject
	{
	public:
		void SetMethods(std::unordered_map <std::string, Ref<ScriptMethod>> methods);
		void SetFields(std::unordered_map <std::string, Ref<ScriptClassField>> fields);
		void SetProperties(std::unordered_map <std::string, Ref<ScriptProperty>> props);
		void SetHandle(MonoObject* handle)
		{
			_Handle = handle;
		}
		MonoObject* GetHandle()
		{
			return _Handle;
		}
		Ref<ScriptClass> GetClass()
		{
			return _scriptclass;
		}
		void SetClass(const Ref<ScriptClass>& klass)
		{
			_scriptclass = klass;
		}
		void Invoke(const std::string& name, void* Args = nullptr);
		void SetField(const std::string name, void* value)
		{
			if (_Fields.find(name) != _Fields.end())
			{
				_Fields[name]->Set<void*>(value);
			}
			else
			{
				CORE_LOG("field not found");
			}
		}
		void SetProp(const std::string name, void* value)
		{
			if (_Properties.find(name) != _Properties.end())
			{
				_Properties[name]->Set<void*>(value);
			}
			else
			{
				MonoClass* _pClass = mono_object_get_class(_Handle);
				MonoProperty* _pProp = nullptr;
				_pProp = mono_class_get_property_from_name(_pClass, name.c_str());
				if (_pProp == nullptr)
				{
					CORE_LOG("couldn't find property with name : " << name);
				}

			}
		}
	private:
		
		MonoObject* _Handle;
		Ref<ScriptClass> _scriptclass;
		std::unordered_map <std::string, Ref<ScriptMethod>> _Methods;
		std::unordered_map <std::string, Ref<ScriptClassField>> _Fields;
		std::unordered_map <std::string, Ref<ScriptProperty>> _Properties;

	};
}