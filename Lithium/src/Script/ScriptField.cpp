#include "lipch.h"
#include "Script/ScriptField.h"


namespace Lithium
{

	ScriptField::ScriptField(MonoClassField* field, MonoObject* object)
		:m_MonoField(field),m_MonoObject(object)
	{
		MonoType* type = mono_field_get_type(field);
		m_Type = GetFieldType(type);
		m_Name = mono_field_get_name(field);
	}

	ScriptType ScriptField::GetType()
	{
		return m_Type;
	}


	void ScriptField::SetMonoValue(void* value)
	{
		mono_field_set_value(m_MonoObject, m_MonoField, value);
	}

	void ScriptField::GetMonoValue(void* val)
	{
		mono_field_get_value(m_MonoObject, m_MonoField, val);
	}

}
