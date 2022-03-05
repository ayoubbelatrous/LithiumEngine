#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
namespace Lithium
{
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(MonoClass* monoclass);
		MonoClass* GetClassPtr();
	private:
		MonoClass* m_MonoClass;
	};
}