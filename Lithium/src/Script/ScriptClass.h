#pragma once
#include "Core/Base.h"
#include "Script/ScriptClassField.h"
#include "Script/ScriptObject.h"


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	
	class ScriptClass
	{
		
	private:
		std::string _Name;
		MonoClass* _Handle;
		MonoDomain* _DomainHandle;
		std::unordered_map <std::string,uint32_t> _Properties;
		std::unordered_map <std::string,uint32_t> _Methods;
		std::unordered_map <std::string,Ref<ScriptClassField>> _Fields;
		
	public:
		static Ref<ScriptObject> CreateInstance(const Ref<ScriptClass>& klass);
		static void InitObjectRuntime(const Ref<ScriptObject>& object);
		ScriptClass(MonoClass* handle, MonoDomain* domainHandle)
			:_Handle(handle), _DomainHandle(domainHandle)
		{}
		ScriptClass() = default;
		void SetName(const std::string& name);
		std::string GetName();
		MonoClass* GetHandle();
		MonoDomain* GetDomainHandle();
		void Reload();
		void LoadFields();
		std::unordered_map <std::string, Ref<ScriptClassField>> GetFields();
	};
}