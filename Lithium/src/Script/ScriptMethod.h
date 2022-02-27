#pragma once
#include "Core/Base.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class ScriptObject;
	class ScriptMethod
	{
		MonoMethod* _Handle;
	public:
		ScriptMethod(MonoMethod* method)
			:_Handle(method)
		{}
		inline void SetHandle(MonoMethod* handle)
		{_Handle = handle;}
		inline MonoMethod* GetHandle()
		{return _Handle;}
		void Invoke(void* params, ScriptObject* object = nullptr);
	};
}