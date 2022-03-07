#include "lipch.h"
#include "Script/ScriptObject.h"


namespace Lithium
{
	ScriptObject::ScriptObject(MonoObject* monoObject)
		:m_MonoObject(monoObject)
	{
		m_ClassName = mono_class_get_name(mono_object_get_class(m_MonoObject));
		LoadFields();
		LoadMethods();
	}

	void ScriptObject::LoadFields()
	{
		void* iter = nullptr;
		MonoClassField* MonoField;
		MonoClass* klass = mono_object_get_class(m_MonoObject);
		
			
		while ((MonoField = mono_class_get_fields(klass, &iter)) != nullptr)

		{
			if ((mono_field_get_flags(MonoField) & MONO_FIELD_ATTR_PUBLIC) == 0)
				continue;
			std::string FieldName = mono_field_get_name(MonoField);
			Ref<ScriptField> Scriptfield = CreateRef<ScriptField>(MonoField, m_MonoObject);
			m_FieldMap.emplace(FieldName, Scriptfield);
		}
	}

	void ScriptObject::LoadMethods()
	{
		void* iter = nullptr;
		MonoMethod* Monomethod;
		MonoClass* klass = mono_object_get_class(m_MonoObject);


		while ((Monomethod = mono_class_get_methods(klass, &iter)) != nullptr)

		{
			std::string methodName = mono_method_get_name(Monomethod);
			Ref<ScriptMethod> scriptMethod = CreateRef<ScriptMethod>(Monomethod,m_MonoObject);
			m_MethodMap.emplace(methodName, scriptMethod);
		}
	}

	void* ScriptObject::InvokeMethod(const std::string& name, void* Params)
	{
		if (m_MethodMap.find(name) != m_MethodMap.end())
		{
			return m_MethodMap[name]->Invoke(Params);
		}
		return nullptr;
	}

}
