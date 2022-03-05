#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>

namespace Lithium
{
	class ScriptObject
	{
	public:
		ScriptObject() = default;
		ScriptObject(MonoObject* object);
	private:
		MonoObject* m_MonoObject;
	};
}