#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>


namespace Lithium
{
	class ScriptProperty
	{
		MonoObject* m_Object = nullptr;
		MonoProperty* m_Prop = nullptr;
	public:
		ScriptProperty(MonoProperty* prop, MonoObject* object)
			:m_Object(object), m_Prop(prop)
		{}
		MonoProperty* GetPropPtr()
		{
			return m_Prop;
		}
		
		template<typename T>
		void Set(T value)
		{
			mono_property_set_value(m_Prop,m_Object, &value,nullptr);
		}
		template<typename T>
		T Get()
		{
			T value = nullptr;
			mono_property_get_value(m_Prop, m_Object, &value, nullptr);
			return value;
		}
	};
}
