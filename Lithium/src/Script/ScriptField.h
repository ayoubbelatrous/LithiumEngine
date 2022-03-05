#pragma once
#include "Core/Base.h"
#include "Script/ScriptTypes.h"
#include <variant>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

namespace Lithium
{
	using FieldValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4,MonoObject*>;
	class ScriptField
	{
	public:

		ScriptField(MonoClassField* field,MonoObject* object)
			:m_Field(field), m_Object(object)
		{
			m_Type = Types::GetFieldType(mono_field_get_type(field));
			m_Name = mono_field_get_name(field);
		}
		
		void SetValue(FieldValue value)
		{
			mono_field_set_value(m_Object, m_Field, &value);
			m_Value = value;
			
		}
	
		FieldValue GetValue()
		{
			switch (m_Type)
			{
			case Lithium::Types::ScriptType::Int:
			{
				FieldValue val;
				mono_field_get_value(m_Object, m_Field, &std::get<int>(val));
				m_Value = val;
				return val;
				break;
			}
				
			case Lithium::Types::ScriptType::Float:
			{
				FieldValue val;
				mono_field_get_value(m_Object, m_Field, &std::get<float>(val));
				m_Value = val;
				return val;
				break;
			}

			case Lithium::Types::ScriptType::Vec2:
			{
				FieldValue val;
				mono_field_get_value(m_Object, m_Field, &std::get<glm::vec2>(val));
				m_Value = val;
				return val;
				break;
			}
			default:
				break;
			}
		
			
		}
		FieldValue GetValueLocal()
		{
			return m_Value;
		}

		void SetValueLocal(FieldValue value)
		{
			m_Value = value;
		}
		std::string GetName()
		{
			return m_Name;
		}
		Types::ScriptType GetType()
		{
			return m_Type;
		}
	private:
		Types::ScriptType m_Type;
		std::string m_Name;
		uint64_t m_UUIDRefrence = 0;

		MonoClassField* m_Field = nullptr;
		MonoObject* m_Object = nullptr;

		FieldValue m_Value;
	};
}