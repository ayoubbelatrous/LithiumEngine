#pragma once
#include "Core/Base.h"


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{

	class ScriptClass;

	class ScriptObject
	{
	public:
		
		void SetHandle(MonoObject* handle)
		{
			_Handle = handle;
		}
		MonoObject* GetHandle()
		{
			return _Handle;
		}
		Ref<ScriptClass> GetClass()
		{
			return _scriptclass;
		}
		void SetClass(const Ref<ScriptClass>& klass)
		{
			_scriptclass = klass;
		}
		void Invoke();
	private:
		
		MonoObject* _Handle;
		Ref<ScriptClass> _scriptclass;
	};
}