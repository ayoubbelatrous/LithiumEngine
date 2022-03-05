#pragma once
#include "Script/ScriptField.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class ScriptObject
	{
	public:
		
		ScriptObject(MonoObject* object);
		std::unordered_map<std::string, Ref<ScriptField>>& GetFields()
		{
			return m_FieldMap;
		}

		void SetFields(const std::unordered_map<std::string, Ref<ScriptField>>& fieldmap)
		{
			m_FieldMap = fieldmap;
		}

		void LoadFields();
	private:
		MonoObject* m_MonoObject = nullptr;
		std::unordered_map<std::string, Ref<ScriptField>> m_FieldMap;
	};
}