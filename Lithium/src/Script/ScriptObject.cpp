#include "lipch.h"

#include "Script/ScriptObject.h"
#include "Script/ScriptClass.h"




namespace Lithium
{
	
	void ScriptObject::SetMethods(std::unordered_map <std::string, Ref<ScriptMethod>> methods)
	{
		_Methods = methods;
	}

	void ScriptObject::SetFields(std::unordered_map <std::string, Ref<ScriptClassField>> fields)
	{
		std::unordered_map<std::string, Ref<ScriptClassField>> newfields;
		for (auto field : fields)
		{
			Ref<ScriptClassField> f = CreateRef<ScriptClassField>(field.second->_fieldHandle, _Handle);
			f->_ScriptClass = field.second->_ScriptClass;
			f->CheckIfSubClassOfScript();
			newfields.emplace(field.first, f);
		}
		_Fields = newfields;
	}

	void ScriptObject::SetProperties(std::unordered_map <std::string, Ref<ScriptProperty>> props)
	{
		std::unordered_map<std::string, Ref<ScriptProperty>> newProps;
		for (auto prop : props)
		{
			Ref<ScriptProperty> p = CreateRef<ScriptProperty>(prop.second->GetPropPtr(), _Handle);
			
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