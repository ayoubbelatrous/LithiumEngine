#pragma once
#include "Core/Base.h"
#include "Script/ScriptTypes.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class ScriptClassField
	{
		void* m_Value;
		Types::ScriptType m_Type;
		MonoClass* m_ScriptClass = nullptr;
		MonoClassField* m_FieldHandle = nullptr;
		MonoObject* m_Object = nullptr;
	public:
		std::string name;
		bool IsSubClassOfComponent= false;

		ScriptClassField(MonoClassField* field, MonoObject* object)
			:m_FieldHandle(field), m_Object(object)
		{
			
		}

		
		void SetClass(MonoClass* klass);
		void SetObject(MonoObject* object);
		void SetFieldHandle(MonoClassField* field);
		void SetType(Types::ScriptType type)
		{
			m_Type = type;
		}
		MonoClass* GetClass();
		MonoObject* GetObject();
		MonoClassField* GetFieldHandle();
		Types::ScriptType GetType() { return m_Type; }

		template<typename T>
		void Set(T value)
		{
			m_Value = value;
			mono_field_set_value(m_Object, m_FieldHandle, value);
		}
		template<typename T>
		T Get()
		{
			T value = nullptr;
			mono_field_get_value(m_Object, m_FieldHandle, &value);
			return value;
		}

		void CheckIfSubClassOfScript();

	};
}