#include "epch.h"
#include "ImguiExtended.h"
#include "Commands/FloatCommand.h"
#include "Commands/CommandHistroy.h"


namespace Lithium
{
	//undo enabled drag float
	bool LiGui::UndoDragFloat(const char* label, float* Value, float speed /*= 1.0F*/)
	{

		float tmp = *Value;
	
		bool result = ImGui::DragFloat(label, &tmp,speed);
		if (result)
		{
		    CommandHistory::AddCommand(new FloatCommand(*Value, tmp));
		}
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			CommandHistory::SetNoMergeMostRecent();
		}
		return result;
	}
}
