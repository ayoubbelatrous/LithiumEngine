#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class ScriptMethod
	{
	public:
		ScriptMethod(MonoMethod* m_Method,MonoObject* m_Object);

		//TODO: add a param list class and use it for args and return values
		void* Invoke(void** params);
	private:
		MonoMethod* m_MonoMethod = nullptr;
		MonoObject* m_MonoObject = nullptr;
	};
}