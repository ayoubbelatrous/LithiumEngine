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
			return GetMonoString();
		}


		template<typename T>
		T GetLocalValue()
		{
			T val;
			switch (m_Type)
			{
			case Lithium::ScriptType::Int:
				ASSERT(m_Type == ScriptType::Int);
				val = std::get<int>(m_Value);
				break;
			case Lithium::ScriptType::Float:
				ASSERT(m_Type == ScriptType::Float);
				val = std::get<float>(m_Value);
				break;
			case Lithium::ScriptType::Vec2:
				ASSERT(m_Type == ScriptType::Vec2);
				val = std::get<glm::vec2>(m_Value);
				break;
			case Lithium::ScriptType::Vec3:
				ASSERT(m_Type == ScriptType::Vec3);
				val = std::get<glm::vec3>(m_Value);
				break;
			case Lithium::ScriptType::Vec4:
				ASSERT(m_Type == ScriptType::Vec4);
				val = std::get<glm::vec4>(m_Value);
				break;
			case Lithium::ScriptType::String:
				return std::get<std::string>(m_Value);
				break;
			}
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