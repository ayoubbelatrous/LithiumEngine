#pragma once
#include "Core/UUID.h"

namespace Lithium
{
	class Asset
	{
		UUID m_Id;
	public:
		Asset()
			:m_Id(UUID(0))
		{
		}
		Asset(UUID id)
			:m_Id(id)
		{
		}

		UUID GetUUID()
		{
			return m_Id;
		}

		void SetUUID(UUID id){
			m_Id = id;
		}

	};
}

