#include "lipch.h"
#include "OpenglUtils.h"
#include "glad/glad.h"



namespace Lithium
{
	namespace Utils
	{
		namespace Opengl
		{
			
			MaterialDataType GltToMdt(uint32_t type)
			{
				if (type == GL_FLOAT_VEC2)
					return MaterialDataType::Vec2;
				if (type == GL_FLOAT_VEC3)
					return MaterialDataType::Vec3;
				if (type == GL_FLOAT_VEC4)
					return MaterialDataType::Vec4;
				if (type == GL_SAMPLER_2D)
					return MaterialDataType::TexturePath;
				else
					return MaterialDataType::NONE;
			}
		}
	}
}


