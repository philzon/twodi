#ifndef ENGINE_HPP
#define ENGINE_HPP

// STD.
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// SDL2.
#include <SDL2/SDL.h>

// Engine.
#include "Window.hpp"
#include "Log.hpp"
#include "INI.hpp"
#include "Utility.hpp"

#ifndef BUILD_TITLE
#define BUILD_TITLE "app"
#endif
#ifndef BUILD_VERSION
#define BUILD_VERSION "0.0.0"
#endif
#ifndef BUILD_COMMIT
#define BUILD_COMMIT "0000000"
#endif
#ifndef BUILD_BRANCH
#define BUILD_BRANCH "None"
#endif
#ifndef BUILD_BUILD
#define BUILD_BUILD "0"
#endif
#ifndef BUILD_DATE
#define BUILD_DATE "1970-01-01 00:00:00"
#endif

// Engine default constants.
#define ENGINE_TPS 60.0f

class Engine
{
public:
	Engine(const std::vector<std::string> &args = std::vector<std::string>());
	virtual ~Engine();

	int run();

private:
	void configure(const std::string &pasth);
	void parse(const std::vector<std::string> &args);

	unsigned int mTick;
	unsigned int mFPS;
	unsigned int mTPS;

	Window mWindow;
	SDL_Event mEvent;

	INI mIni;

	bool mFramelock;
	bool mDebug;

	std::shared_ptr<Log> mLog;
};

#endif // ENGINE_HPP
