#pragma once
#include "Core/Base.h"
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
	public:
		int type;
		std::string name;
		MonoClassField* _fieldHandle = nullptr;
		MonoObject* _Object = nullptr;
		MonoClass* _ScriptClass = nullptr;
		bool IsSubClassOfComponent= false;
		ScriptClassField(MonoClassField* field, MonoObject* object)
			:_fieldHandle(field), _Object(object)
		{
			
		}
		template<typename T>
		void Set(T value)
		{
			mono_field_set_value(_Object, _fieldHandle, value);
		}
		template<typename T>
		T Get()
		{
			T value = nullptr;
			mono_field_get_value(_Object, _fieldHandle, &value);
			return value;
		}

		void CheckIfSubClassOfScript();

	};
}