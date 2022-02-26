#include "lipch.h"

#include "Script/ScriptObject.h"
#include "Script/ScriptClass.h"




namespace Lithium
{
	
	void ScriptObject::Invoke()
	{
		MonoMethodDesc* desc;
		desc = mono_method_desc_new("Test:Update()", false);
		if (!desc)
		{
			CORE_LOG("method not found!!");
		}
		else
		{
			MonoMethod* method = mono_method_desc_search_in_class(desc, _scriptclass->GetHandle());

			mono_runtime_invoke(method, _Handle, nullptr, nullptr);
		}

	}

}