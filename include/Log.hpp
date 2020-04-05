#ifndef LOG_HPP
#define LOG_HPP

#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#define TIME_FORMAT "%H:%M:%S"

class Log
{
public:
	static std::shared_ptr<Log> getInstance()
	{
		std::shared_ptr<Log> instance(new Log());
		return instance;
	}

	void debug(const std::string &message);
	void info(const std::string &message);
	void warning(const std::string &message);
	void error(const std::string &message);
	void critical(const std::string &message);

private:
	Log();
	Log(const Log &other) = delete;
	void operator=(const Log &other) = delete;
	std::string time(const std::string &format) const;
};

#endif // LOG_HPP
