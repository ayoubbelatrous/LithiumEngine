#pragma once

#include "ScriptTypes.h"
#include "Core/Base.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <variant>

namespace Lithium
{
	using FieldValue = std::variant<int,float,glm::vec2,glm::vec3,glm::vec4,std::string>;

	class ScriptField
	{
	public:
		ScriptField() = default;
		ScriptField(MonoClassField* field,MonoObject* object);
		ScriptType GetType();
		bool Initilized = false;

		template<typename T>
		void SetValue(T value)
		{
			ASSERT(false);
		}

		template<typename T>
		T GetValue()
		{
			 T val;
			 GetMonoValue(&val);
			 return val;
		}

		template<>
		std::string GetValue()
		{
			std::string val = GetMonoString();
			return val;
		}


		template<typename T>
		T GetLocalValue()
		{
			return std::get<T>(m_Value);
		}

		std::string GetLocalValue()
		{
			return std::get<std::string>(m_Value);
		}

		template<>
		void SetValue(int value)
		{
			ASSERT(m_Type == ScriptType::Int);
			Initilized = true;
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(float value)
		{
			ASSERT(m_Type == ScriptType::Float);
			m_Value = value;
			Initilized = true;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec2 value)
		{
			ASSERT(m_Type == ScriptType::Vec2);
			m_Value = value;
			Initilized = true;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec3 value)
		{
			ASSERT(m_Type == ScriptType::Vec3);
			m_Value = value;
			Initilized = true;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec4 value)
		{
			ASSERT(m_Type == ScriptType::Vec4);
			m_Value = value;
			Initilized = true;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(std::string value)
		{
			ASSERT(m_Type == ScriptType::String);
			m_Value = value;
			Initilized = true;
			SetMonoString(value);
		}


	private:
		void SetMonoValue(void* value);
		void SetMonoString(const std::string& value);
		void GetMonoValue(void* val);
		std::string GetMonoString();
		std::string m_Name;
		MonoClassField* m_MonoField = nullptr;
		MonoObject* m_MonoObject = nullptr;
		ScriptType m_Type;
		FieldValue m_Value;


	};

	

}