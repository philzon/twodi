#ifndef ENGINE_HPP
#define ENGINE_HPP

// STD.
#include <iostream>
#include <vector>
#include <string>

// SDL2.
#include <SDL2/SDL.h>

// Engine.
#include "INI.hpp"

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

// Window default constants.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_POSITION SDL_WINDOWPOS_CENTERED
#define WINDOW_STATES SDL_WINDOW_SHOWN

// Renderer default constants.
#define RENDERER_INDEX -1
#define RENDERER_CONTEXT SDL_RENDERER_ACCELERATED

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

	// SDL systems.
	SDL_Window *mSDLWindow;
	SDL_Renderer *mSDLRenderer;
	SDL_Event mEvent;

	// Flags and configuration values.
	int mWindowPosition;
	int mWindowWidth;
	int mWindowHeight;
	unsigned int mWindowStates;

	int mRenderingIndex;
	unsigned int mRenderingContext;

	bool mFramelock;
	bool mDebug;
};

#endif // ENGINE_HPP
