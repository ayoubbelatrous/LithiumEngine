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
		T GetLocalValue()
		{
		}


		template<typename T>
		T GetValue()
		{
			 T val;
			 GetMonoValue(&val);
			 return val;
		}
		template<typename T>
		void SetValue(T value)
		{
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