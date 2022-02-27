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
			f->_ComponentClass = field.second->_ComponentClass;
			f->CheckIfSubClassOfComponent();
			newfields.emplace(field.first, f);
		}
		_Fields = newfields;
	}

	void ScriptObject::Invoke(const std::string& name)
	{
		if (_Methods.find(name) != _Methods.end())
		{
			_Methods[name]->Invoke(nullptr,this);
		}
		else
		{
			CORE_LOG("method" << name << " not found");
		}


	}

}