#pragma once

#include "ScriptTypes.h"
#include "Core/UUID.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <variant>

namespace Lithium
{
	using FieldValue = std::variant<int,float,bool,glm::vec2,glm::vec3,glm::vec4,std::string,uint64_t>;

	class ScriptField
	{
	public:
		ScriptField() = default;
		ScriptField(MonoClassField* field,MonoObject* object);
		ScriptType GetType();
		

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

		template<>
		uint64_t GetValue()
		{
			uint64_t val = GetMonoEntity();
			return val;
		}

		template<>
		UUID GetValue()
		{
			uint64_t val = GetMonoAudioClip();
			return UUID(val);
		}


		template<typename T>
		T GetLocalValue()
		{
			return std::get<T>(m_Value);
		}
		template<>
		std::string GetLocalValue()
		{
			return std::get<std::string>(m_Value);
		}
		template<>
		uint64_t GetLocalValue()
		{
			return std::get<uint64_t>(m_Value);
		}
		template<>
		UUID GetLocalValue()
		{
			return std::get<uint64_t>(m_Value);
		}


		template<>
		void SetValue(int value)
		{
			ASSERT(m_Type == ScriptType::Int);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(float value)
		{
			ASSERT(m_Type == ScriptType::Float);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(bool value)
		{
			ASSERT(m_Type == ScriptType::Bool);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec2 value)
		{
			ASSERT(m_Type == ScriptType::Vec2);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec3 value)
		{
			ASSERT(m_Type == ScriptType::Vec3);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(glm::vec4 value)
		{
			ASSERT(m_Type == ScriptType::Vec4);
			m_Value = value;
			SetMonoValue(&value);
		}

		template<>
		void SetValue(std::string value)
		{
			ASSERT(m_Type == ScriptType::String);
			m_Value = value;
			SetMonoString(value);
		}

		template<>
		void SetValue(uint64_t value)
		{
			ASSERT(m_Type == ScriptType::Entity);
			m_Value = value;
			SetMonoEntity(std::get<uint64_t>(m_Value));
		}
		template<>
		void SetValue(UUID assetID)
		{
			ASSERT(m_Type == ScriptType::AudioClip);
			m_Value = assetID;
			SetMonoAudioClip(std::get<uint64_t>(m_Value));
		}



	private:

		//mono any type helpers
		void SetMonoValue(void* value);
		void GetMonoValue(void* val);
		//mono string helpers
		void SetMonoString(const std::string& value);
		std::string GetMonoString();
		//entity field helpers
		void SetMonoEntity(uint64_t uuid);
		void SetMonoAudioClip(uint64_t uuid);
		uint64_t GetMonoEntity();
		uint64_t GetMonoAudioClip();
		std::string m_Name;
		MonoClassField* m_MonoField = nullptr;
		MonoObject* m_MonoObject = nullptr;
		ScriptType m_Type;
		FieldValue m_Value;


	};

	

}