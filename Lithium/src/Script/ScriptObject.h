#pragma once
#include "Core/Base.h"
#include "Script/ScriptField.h"
#include "Script/ScriptMethod.h"
#include "Script/ScriptProperty.h"

namespace Lithium
{

	class ScriptClass;

	class ScriptObject
	{
	public:

		ScriptObject(MonoObject* object);

		void SetMethods(std::unordered_map <std::string, Ref<ScriptMethod>> methods);
		void SetProperties(std::unordered_map <std::string, Ref<ScriptProperty>> props);
		void Invoke(const std::string& name, void* Args = nullptr);
		template<typename T>
		void SetProp(const std::string name, T value)
		{
			if (_Properties.find(name) != _Properties.end())
			{
				_Properties[name]->Set(value);
			}
			else
			{
				MonoClass* _pClass = mono_object_get_class(m_Object);
				MonoProperty* _pProp = nullptr;
				_pProp = mono_class_get_property_from_name(_pClass, name.c_str());
				_Properties.emplace(name, CreateRef<ScriptProperty>(_pProp, m_Object));
				if (_pProp == nullptr)
				{
					CORE_LOG("couldn't find property with name : " << name);
				}
				_Properties[name]->Set(value);
			}
		}
		MonoObject* GetObjectPtr() { return m_Object; }
		
		void SetField(const std::string& name,FieldValue value)
		{
			if (m_Fields.find(name) != m_Fields.end())
			{
				m_Fields[name]->SetValue(value);
			}
			else
			{
				CORE_LOG(" field : " << name << " not found");
			}
		}

		std::unordered_map <std::string, Ref<ScriptField>>& GetFields() { return m_Fields; }
	private:
		MonoObject* m_Object;
		MonoClass* m_Class;
		std::unordered_map <std::string, Ref<ScriptMethod>> _Methods;
		std::unordered_map <std::string, Ref<ScriptProperty>> _Properties;
		std::unordered_map <std::string, Ref<ScriptField>> m_Fields;

	};
}