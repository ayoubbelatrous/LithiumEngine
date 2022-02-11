#pragma once
#include "Mesh/Material.h"


namespace Lithium
{
	namespace Utils
	{
		namespace Opengl
		{
			//converts opengl type to MaterialDataType
			MaterialDataType GltToMdt(uint32_t type);
			
		}
	}
}