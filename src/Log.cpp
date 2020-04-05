#include "Log.hpp"

void Log::debug(const std::string &message)
{
	std::cout << time(TIME_FORMAT) << " DEBU " << message << "\n";
}

void Log::info(const std::string &message)
{
	std::cout << time(TIME_FORMAT) << " INFO " << message << "\n";
}

void Log::warning(const std::string &message)
{
	std::cout << time(TIME_FORMAT) << " WARN " << message << "\n";
}

void Log::error(const std::string &message)
{
	std::cout << time(TIME_FORMAT) << " ERRO " << message << "\n";
}

void Log::critical(const std::string &message)
{
	std::cout << time(TIME_FORMAT) << " CRIT " << message << "\n";
}

////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////

Log::Log()
{}

std::string Log::time(const std::string &format) const
{
	time_t time = std::time(nullptr);
	tm local = *std::localtime(&time);

	std::ostringstream ss;
	ss << std::put_time(&local, format.c_str());

	return ss.str();
}
