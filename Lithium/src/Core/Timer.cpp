#include "lipch.h"
#include "Timer.h"


namespace Lithium
{

	void Timer::SetInterval(std::function<void()> function, std::chrono::milliseconds ms)
	{
		std::thread t([=]() {
			while (!stop) {
				std::this_thread::sleep_for(ms);
				function();
			}
		});	
		t.detach();
	}

}