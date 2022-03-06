#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	
	enum class ScriptType
	{
		None = 0,
		Float, Int, Char, String, Bool,
		Vec2, Vec3, Vec4,

		Transform
	};


	static ScriptType GetFieldType(MonoType* monoType)
	{
		int type = mono_type_get_type(monoType);
		switch (type)
		{
		case MONO_TYPE_R4: return ScriptType::Float;
		case MONO_TYPE_I4: return ScriptType::Int;
		case MONO_TYPE_STRING: return ScriptType::String;
		case MONO_TYPE_VALUETYPE:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "Lithium.Math.Vector2") == 0) return ScriptType::Vec2;
			if (strcmp(name, "Lithium.Math.Vector3") == 0) return ScriptType::Vec3;
		}
		case MONO_TYPE_CLASS:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "Lithium.Core.Transform") == 0) return ScriptType::Transform;

		}
		}
		return ScriptType::None;
	}

	
}