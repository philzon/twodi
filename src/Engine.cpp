#include "Engine.hpp"

Engine::Engine(const std::vector<std::string> &args)
{
	mFPS = 0;
	mTPS = 0;
	mTick = 0;

	mSDLWindow = nullptr;
	mSDLRenderer = nullptr;

	mWindowTitle = WINDOW_TITLE;
	mWindowPosition = WINDOW_POSITION;
	mWindowWidth = WINDOW_WIDTH;
	mWindowHeight = WINDOW_HEIGHT;
	mWindowStates = WINDOW_STATES;

	mRenderingIndex = RENDERER_INDEX;
	mRenderingContext = RENDERER_CONTEXT;
	mFramelock = true;

	mDebug = false;

	configure("config.ini");
	parse(args);

	SDL_Init(SDL_INIT_EVERYTHING);

	mSDLWindow = SDL_CreateWindow(mWindowTitle.c_str(),
		mWindowPosition,
		mWindowPosition,
		mWindowWidth,
		mWindowHeight,
		mWindowStates);
	mSDLRenderer = SDL_CreateRenderer(mSDLWindow,
		mRenderingIndex,
		mRenderingContext);
}

Engine::~Engine()
{
	SDL_DestroyRenderer(mSDLRenderer);
	SDL_DestroyWindow(mSDLWindow);
	SDL_Quit();
}

int Engine::run()
{
	unsigned int last = 0;
	double unprocessed = 0.0f;
	bool running = true;

	while (running)
	{
		unprocessed += (SDL_GetTicks() - last) / (1000.0f / ENGINE_TPS);
		last = SDL_GetTicks();

		while (SDL_PollEvent(&mEvent))
		{
			switch (mEvent.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		// Always render if framelock is disabled.
		bool shouldRender = !mFramelock;
		bool shouldUpdate = false;

		while (unprocessed >= 1.0f)
		{
			unprocessed -= 1.0f;
			shouldRender = true;
			shouldUpdate = true;

			// Perform logic operations here ...

			++mTick;
			++mTPS;
		}

		if (shouldRender)
		{
			SDL_RenderClear(mSDLRenderer);
			SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 255);
			SDL_RenderFillRect(mSDLRenderer, NULL);

			// Perform rendering operations here ...

			SDL_RenderPresent(mSDLRenderer);

			++mFPS;
		}

		// Update loop information each second.
		if (shouldUpdate && mTick % 60 == 0)
		{
			// Refresh debug title each second.
			if (mDebug)
			{
				int width = 0;
				int height = 0;

				SDL_GetWindowSize(mSDLWindow, &width, &height);

				mWindowTitle = std::string(BUILD_TITLE) \
					+ std::string(" ") \
					+ std::string(BUILD_VERSION) \
					+ std::string("-") \
					+ std::string(BUILD_BUILD) \
					+ std::string(" (") \
					+ std::string(BUILD_COMMIT) \
					+ std::string(" -> ") \
					+ std::string(BUILD_BRANCH) \
					+ std::string(") | ") \
					+ std::to_string(width) \
					+ std::string("x") \
					+ std::to_string(height) \
					+ std::string(" | ") \
					+ std::to_string(mFPS) \
					+ std::string(" FPS | ") \
					+ std::to_string(mTPS) \
					+ std::string(" TPS | ") \
					+ std::to_string(mTick) \
					+ std::string(" Tick");

				SDL_SetWindowTitle(mSDLWindow, mWindowTitle.c_str());
			}

			mFPS = 0;
			mTPS = 0;
		}
	}

	return 0;
}

void Engine::parse(const std::vector<std::string> &args)
{
	for (unsigned int i = 0; i < args.size(); ++i)
	{
		std::string flag = args.at(i);

		if (flag == "-d" || flag == "--debug")
			mDebug = true;
	}
}

void Engine::configure(const std::string &path)
{
	INI ini(path);

	if (!ini.getErrors().empty())
	{
		std::cout << "Config contains errors: " << path << "\n";
		std::cout << ini.getErrors();
	}

	mWindowWidth = ini.getInteger("WINDOW", "iWidth", mWindowWidth);
	mWindowHeight = ini.getInteger("WINDOW", "iHeight", mWindowHeight);

	bool fullscreen = ini.getBoolean("WINDOW", "bFullscreen", false);
	bool borderless = ini.getBoolean("WINDOW", "bBorderless", false);
	bool resizeable = ini.getBoolean("WINDOW", "bResizeable", false);

	if (fullscreen)
		mWindowStates |= SDL_WINDOW_FULLSCREEN;
	if (borderless)
		mWindowStates |= SDL_WINDOW_BORDERLESS;
	if (resizeable)
		mWindowStates |= SDL_WINDOW_RESIZABLE;

	bool vsync = ini.getBoolean("RENDERER", "bVsync", false);
	bool software = ini.getBoolean("RENDERER", "bSoftware", false);
	mFramelock = ini.getBoolean("RENDERER", "bFramelock", mFramelock);

	if (vsync)
		mRenderingContext |= SDL_RENDERER_PRESENTVSYNC;
	if (software)
		mRenderingContext |= SDL_RENDERER_SOFTWARE;

	mDebug = ini.getBoolean("ENGINE", "bDebug", mDebug);
}
