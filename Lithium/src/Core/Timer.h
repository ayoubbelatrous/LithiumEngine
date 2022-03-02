#pragma once
#include <chrono>

namespace Lithium
{
	class Timer
	{
		
	public:
		void SetInterval(std::function<void()> function, std::chrono::milliseconds ms);
		void Stop(){stop = true;}
		void Resume() { stop = false; }
	private:
		bool stop = false;
		
	};
}