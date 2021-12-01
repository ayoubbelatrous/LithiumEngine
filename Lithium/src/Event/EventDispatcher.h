#pragma once
#include "Event.h"



namespace Lithium
{
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			:e(e)
		{

		}
		template<typename T,typename F>
		void Dispatch(const F& function)
		{
			function(static_cast<T&>(e));
		}


		Event& e;
	};
}
