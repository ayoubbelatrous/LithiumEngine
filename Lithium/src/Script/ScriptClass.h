#pragma once
#include "Core/Base.h"
#include "Script/ScriptClassField.h"

namespace Lithium
{
	class ScriptClass
	{
		
	private:
		std::string _Name;
		uint32_t _Handle;
		std::unordered_map <std::string,uint32_t> _Properties;
		std::unordered_map <std::string,uint32_t> _Methods;
		std::unordered_map <std::string,Ref<ScriptClassField>> _Fields;
		
	public:
		ScriptClass(uint32_t handle)
			:_Handle(handle)
		{}
		ScriptClass() = default;
		void SetName(const std::string& name);
		std::string GetName();
		uint32_t GetHandle();
		void Reload();
		void LoadFields();
	

		std::unordered_map <std::string, Ref<ScriptClassField>> GetFields();
		
		//void RegenerateCache(); regenerates caches for fields and methods and properties
		//TODO: add methods to get properties
		//TODO: add methods to get fields
		//TODO: add methods to get Script methods
	};
}