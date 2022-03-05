#include "lipch.h"
#include "Script/ScriptObject.h"


namespace Lithium
{
	ScriptObject::ScriptObject(MonoObject* monoObject)
		:m_MonoObject(monoObject)
	{
		LoadFields();
	}

	void ScriptObject::LoadFields()
	{
		void* iter = nullptr;
		MonoClassField* MonoField;
		MonoClass* klass = mono_object_get_class(m_MonoObject);
		
			
		while ((MonoField = mono_class_get_fields(klass, &iter)) != nullptr)

		{
			std::string FieldName = mono_field_get_name(MonoField);
			Ref<ScriptField> Scriptfield = CreateRef<ScriptField>(MonoField, m_MonoObject);
			m_FieldMap.emplace(FieldName, Scriptfield);
		}
	}

}
