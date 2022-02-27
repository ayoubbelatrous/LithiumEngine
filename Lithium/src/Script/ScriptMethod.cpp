#include "lipch.h"
#include "ScriptMethod.h"
#include "Script/ScriptClass.h"

namespace Lithium
{ 

	void ScriptMethod::Invoke(void* params,ScriptObject* object /*= nullptr*/)
	{
		if (object == nullptr)
		{
			return;
		}
		else
		{
			mono_runtime_invoke(_Handle,object->GetHandle(), nullptr, nullptr);
		}
	}

}