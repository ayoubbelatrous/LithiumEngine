#include "lipch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace Lithium
{
	Ref<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{

		s_Logger = spdlog::stdout_color_mt("Lithium");
		s_Logger->set_level(spdlog::level::trace);
		s_Logger->flush_on(spdlog::level::trace);
		s_Logger->set_pattern("%v%$");
	}
}


