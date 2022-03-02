#include "lipch.h"

#include "Script/ScriptObject.h"
#include "Script/ScriptClass.h"




namespace Lithium
{
	
	ScriptObject::ScriptObject(MonoObject* object)
	{
		
		m_Object = object;
		m_Class = mono_object_get_class(object);
		MonoClassField* field;
		void* iter = nullptr;

		while ((field = mono_class_get_fields(m_Class, &iter)) != nullptr)
		{
			if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_PUBLIC) == 0)
				continue;
			std::string name = mono_field_get_name(field);
			m_Fields.emplace(name, CreateRef<ScriptField>(field, m_Object));
		}

	}

	void ScriptObject::SetMethods(std::unordered_map <std::string, Ref<ScriptMethod>> methods)
	{
		_Methods = methods;
	}


	void ScriptObject::SetProperties(std::unordered_map <std::string, Ref<ScriptProperty>> props)
	{
		std::unordered_map<std::string, Ref<ScriptProperty>> newProps;
		for (auto prop : props)
		{
			Ref<ScriptProperty> p = CreateRef<ScriptProperty>(prop.second->GetPropPtr(), m_Object);
			
			newProps.emplace(prop.first, p);
		}
		_Properties = newProps;
	}

	void ScriptObject::Invoke(const std::string& name, void* Args)
	{
		if (_Methods.find(name) != _Methods.end())
		{
			_Methods[name]->Invoke(Args,this);
		}
		else
		{
			CORE_LOG("method" << name << " not found");
		}
	}

}