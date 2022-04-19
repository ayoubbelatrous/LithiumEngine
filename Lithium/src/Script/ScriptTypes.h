#pragma once
#include <algorithm>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <string.h> 
#include <stdio.h>
namespace Lithium
{
	
	enum class ScriptType
	{
		None = 0,
		Float, Int, Char, String, Bool,
		Vec2, Vec3, Vec4,
		Transform,
		Entity,
		AudioClip
	};


	static ScriptType GetFieldType(MonoType* monoType)
	{
		int type = mono_type_get_type(monoType);
		switch (type)
		{
		case MONO_TYPE_R4: return ScriptType::Float;
		case MONO_TYPE_I4: return ScriptType::Int;
		case MONO_TYPE_STRING: return ScriptType::String;
		case MONO_TYPE_BOOLEAN: return ScriptType::Bool;
		case MONO_TYPE_VALUETYPE:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "Lithium.Math.Vector2") == 0) return ScriptType::Vec2;
			if (strcmp(name, "Lithium.Math.Vector3") == 0) return ScriptType::Vec3;
			if (strcmp(name, "Lithium.Math.Vector4") == 0) return ScriptType::Vec4;
		}
		case MONO_TYPE_CLASS:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "Lithium.Core.Entity") == 0) return ScriptType::Entity;
			name = mono_type_get_name(monoType);
			if (strcmp(name, "Lithium.Core.AudioClip") == 0) return ScriptType::AudioClip;
		}
		}
		return ScriptType::None;
	}
	static ScriptType ConvertStringToScriptType(const std::string& str)
	{
		if (strcmp(str.c_str(), "INT") == 0)
		{
			return ScriptType::Int;
		}

		else if (strcmp(str.c_str(), "FLOAT") == 0)
		{
			return ScriptType::Float;
		}

		else if (strcmp(str.c_str(), "BOOL") == 0)
		{
			return ScriptType::Bool;
		}

		else if (strcmp(str.c_str(), "VEC2") == 0)
		{
			return ScriptType::Vec2;
		}

		else if (strcmp(str.c_str(), "VEC3") == 0)
		{
			return ScriptType::Vec3;
		}

		else if (strcmp(str.c_str(), "VEC4") == 0)
		{
			return ScriptType::Vec4;
		}
		else if (strcmp(str.c_str(), "STRING") == 0)
		{
			return ScriptType::String;
		}
		else if (strcmp(str.c_str(), "ENTITY") == 0)
		{
			return ScriptType::Entity;
		}

		return ScriptType::None;
	}
	
}