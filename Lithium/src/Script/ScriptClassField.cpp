#include "lipch.h"
#include "Script/ScriptClassField.h"


namespace Lithium
{

	void ScriptClassField::SetClass(MonoClass* klass)
	{
		m_ScriptClass = klass;
	}

	void ScriptClassField::SetObject(MonoObject* object)
	{
		m_Object = object;
	}

	void ScriptClassField::SetFieldHandle(MonoClassField* field)
	{
		m_FieldHandle = field;
	}

	MonoClass* ScriptClassField::GetClass()
	{
		return m_ScriptClass;
	}

	MonoObject* ScriptClassField::GetObject()
	{
		return m_Object;
	}

	MonoClassField* ScriptClassField::GetFieldHandle()
	{
		return m_FieldHandle;
	}

	void ScriptClassField::CheckIfSubClassOfScript()
	{
		MonoType* type = mono_field_get_type(m_FieldHandle);
		MonoClass * klass = mono_class_from_mono_type(type);
		if (mono_class_is_subclass_of(klass, m_ScriptClass, false))
		{
			CORE_LOG("field : " << name << " is component");
			IsSubClassOfComponent = true;
		}
	}

}