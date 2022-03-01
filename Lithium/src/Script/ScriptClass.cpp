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
		object->SetMethods(klass->GetMethods());
		object->SetFields(klass->GetFields());
		object->SetProperties(klass->GetProperties());
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
			
			Ref<ScriptClassField> _field = CreateRef<ScriptClassField>(field,nullptr);
			MonoType* monotype = mono_field_get_type(field);
			_field->SetType(Types::GetFieldType(monotype));
			_field->name = name;
			_field->SetClass(_ScriptClass);
			_field->CheckIfSubClassOfScript();
			
			_Fields.emplace(name,_field);
			
		}

		MonoMethod* method;
		iter = nullptr;
		_Methods.clear();

		while ((method = mono_class_get_methods(_Handle, &iter)) != nullptr)
		{
			Ref<ScriptMethod> _method = CreateRef<ScriptMethod>(method);
			std::string methodname = mono_method_get_name(method);
			_Methods.emplace(methodname,_method);
		}

		MonoProperty* prop;
		iter = nullptr;
		_Properties.clear();

		while ((prop = mono_class_get_properties(_Handle, &iter)) != nullptr)
		{
			Ref<ScriptProperty> _Prop = CreateRef<ScriptProperty>(prop,nullptr);
			std::string propName = mono_property_get_name(prop);
			_Properties.emplace(propName, _Prop);
		}
		
	}

	std::unordered_map <std::string, Ref<ScriptClassField>> ScriptClass::GetFields()
	{
		return _Fields;
	}

	std::unordered_map <std::string, Ref<ScriptMethod>> ScriptClass::GetMethods()
	{
		return _Methods;
	}

	std::unordered_map <std::string, Ref<ScriptProperty>> ScriptClass::GetProperties()
	{
		return _Properties;
	}

}