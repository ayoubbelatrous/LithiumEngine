#pragma once
#include "Core/Base.h"
#include "Script/ScriptTypes.h"

namespace Lithium
{

	class ScriptField
	{
	public:

		ScriptField(MonoClassField* field,MonoObject* object)
			:m_Field(field), m_Object(object)
		{
			m_Type = Types::GetFieldType(mono_field_get_type(field));
			m_Name = mono_field_get_name(field);
		}
		template<typename T>
		void SetValue(T value)
		{
		}
		template<typename T>
		T GetValue()
		{
		}

		std::string GetName()
		{
			return m_Name;
		}
		

		template<>
		void SetValue(int value)
		{
			mono_field_set_value(m_Object, m_Field, &value);
			m_Value = malloc(4);
			memcpy(m_Value, &value, 4);
		}
		
		template<>
		int GetValue()
		{
			if (m_Value == nullptr)
			{
				int value;
				mono_field_get_value(m_Object, m_Field, &value);
				return value;
			}
			else
			{
				int value;
				memcpy(&value, m_Value, 4);
				return value;
			}
		}
	private:
		Types::ScriptType m_Type;
		std::string m_Name;
		MonoClassField* m_Field = nullptr;
		MonoObject* m_Object = nullptr;
		void* m_Value = nullptr;
	};
}