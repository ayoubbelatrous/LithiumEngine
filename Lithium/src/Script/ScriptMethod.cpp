#include "lipch.h"
#include "ScriptMethod.h"
#include "Script/ScriptClass.h"
#include "Script/MonoServer.h"
#include "Core/Application.h"

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
			MonoObject* exception = nullptr;
			mono_runtime_invoke(_Handle, object->GetObjectPtr(), nullptr, &exception);

			if (exception != nullptr)
			{
// 				MonoProperty* prop = mono_class_get_property_from_name(mono_object_get_class(exception), "Message");
// 				const char* text = mono_string_to_utf8((MonoString*)mono_property_get_value(prop, exception, nullptr, nullptr));
// 				CORE_LOG("exception " << text);
				Application::GetInstance().MonoServer->ForwardMonoException(exception);
			}

			
			
		}
	}

}