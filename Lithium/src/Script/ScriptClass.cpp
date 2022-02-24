#include "lipch.h"
#include "ScriptClass.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>


namespace Lithium
{
	
	void ScriptClass::SetName(const std::string& name)
	{
		_Name = name;
	}

	std::string ScriptClass::GetName()
	{
		return _Name;
	}

	uint32_t ScriptClass::GetHandle()
	{
		return _Handle;
	}

	void ScriptClass::Reload()
	{
		LoadFields();
	}

	void ScriptClass::LoadFields()
	{

		MonoClassField* field;
		void* iter = nullptr;
		_Fields.clear();
		while ((field = mono_class_get_fields((MonoClass*)_Handle, &iter)) != nullptr)
		{
			if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_PUBLIC) == 0)
				continue;
			std::string name = mono_field_get_name(field);
			
			uint32_t type = (uint32_t)mono_field_get_type(field);
			Ref<ScriptClassField> _field = CreateRef<ScriptClassField>();
			_field->name = name;
			_field->type = type;
			_Fields.emplace(name,_field);
		}
		
	}

	std::unordered_map <std::string, Ref<ScriptClassField>> ScriptClass::GetFields()
	{
		return _Fields;
	}

}