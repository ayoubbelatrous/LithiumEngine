#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
#include "Core/Base.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)



namespace Lithium
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static Ref<spdlog::logger> s_Logger;
	};
}
	template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
	{
		return os << glm::to_string(vector);
	}

	template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
	{
		return os << glm::to_string(matrix);
	}

	template<typename OStream, typename T, glm::qualifier Q>
	inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
	{
		return os << glm::to_string(quaternion);
	}

#define LT_CORE_TRACE(...)    ::Lithium::Log::GetLogger()->trace(__VA_ARGS__)
#define LT_CORE_INFO(...)     ::Lithium::Log::GetLogger()->info(__VA_ARGS__)
#define LT_CORE_WARN(...)     ::Lithium::Log::GetLogger()->warn(__VA_ARGS__)
#define LT_CORE_ERROR(...)    ::Lithium::Log::GetLogger()->error(__VA_ARGS__)
#define LT_CORE_CRITICAL(...) ::Lithium::Log::GetLogger()->critical(__VA_ARGS__)

