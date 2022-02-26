#include "lipch.h"

#include "Script/ScriptObject.h"
#include "Script/ScriptClass.h"
#include "glm.hpp"


namespace Lithium
{
	
	Ref<ScriptObject> ScriptClass::CreateInstance(const Ref<ScriptClass>& klass)
	{
		Ref<ScriptObject> object = CreateRef<ScriptObject>();
		object->SetHandle(mono_object_new(klass->_DomainHandle, klass->_Handle));
		object->SetClass(klass);
		glm::vec2 test(10.0f);
		mono_field_set_value(object->GetHandle(), klass->GetFields()["test"]->_fieldHandle,&test);
		return object;
		
 	}

	void ScriptClass::InitObjectRuntime(const Ref<ScriptObject>& object)
	{
		mono_runtime_object_init(object->GetHandle());
	}

	void ScriptClass::SetName(const std::string& name)
	{
		_Name = name;
	}

	std::string ScriptClass::GetName()
	{
		return _Name;
	}

	MonoClass* ScriptClass::GetHandle()
	{
		return _Handle;
	}

	MonoDomain* ScriptClass::GetDomainHandle()
	{
		return _DomainHandle;
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
		while ((field = mono_class_get_fields(_Handle, &iter)) != nullptr)
		{
			if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_PUBLIC) == 0)
				continue;
			std::string name = mono_field_get_name(field);
			
			uint32_t type = (uint32_t)mono_field_get_type(field);
			Ref<ScriptClassField> _field = CreateRef<ScriptClassField>();
			_field->name = name;
			_field->type = type;
			_field->_fieldHandle = field;
			_Fields.emplace(name,_field);
			
		}
		
	}

	std::unordered_map <std::string, Ref<ScriptClassField>> ScriptClass::GetFields()
	{
		return _Fields;
	}

}