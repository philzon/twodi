#include "Engine.hpp"

Engine::Engine(const std::vector<std::string> &args)
{
	mLog = Log::getInstance();
	mLog->info("Starting application: "
		+ std::string(BUILD_TITLE)
		+ " "
		+ std::string(BUILD_VERSION)
		+ " ("
		+ std::string(BUILD_COMMIT)
		+ ")");
	mLog->info("");

	mIni.clear();

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

	mLog->info("");
	mLog->info("Created window (" + hex(mSDLWindow) + ")");

	mSDLRenderer = SDL_CreateRenderer(mSDLWindow,
		mRenderingIndex,
		mRenderingContext);

	mLog->info("Created renderer (" + hex(mSDLRenderer) + ")");

	mLog->info("");
	mLog->info("== Window ==================");
	mLog->info("   Fullscreen  " + mIni.getString("WINDOW", "bFullscreen", "false"));
	mLog->info("   Borderless  " + mIni.getString("WINDOW", "bBorderless", "false"));
	mLog->info("   Resizeable  " + mIni.getString("WINDOW", "bResizeable", "false"));
	mLog->info("   Size        " + std::to_string(mWindowWidth) + "x" + std::to_string(mWindowHeight));
	mLog->info("== Renderer ================");
	mLog->info("   Vsync       " + mIni.getString("RENDERER", "bVsync", "false"));
	mLog->info("   Software    " + mIni.getString("RENDERER", "bSoftware", "false"));
	mLog->info("   Framelock   " + mIni.getString("RENDERER", "bFramelock", "false"));
	mLog->info("== Engine ==================");
	mLog->info("   Debug       " + mIni.getString("ENGINE", "bDebug", "false"));
	mLog->info("");
}

Engine::~Engine()
{
	mLog->info("Destroying renderer (" + hex(mSDLRenderer) + ")");
	SDL_DestroyRenderer(mSDLRenderer);

	mLog->info("Destroying window (" + hex(mSDLWindow) + ")");
	SDL_DestroyWindow(mSDLWindow);

	mLog->info("Destroying SDL");
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
				mLog->info("");
				mLog->info("QUIT event triggered");
				mLog->info("");
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
	mLog->info("Loading " + path + " ...");

	mIni.load(path);

	if (!mIni.getErrors().empty())
	{
		std::cout << "Config contains errors: " << path << "\n";
		std::cout << mIni.getErrors();
	}

	mWindowWidth = mIni.getInteger("WINDOW", "iWidth", mWindowWidth);
	mWindowHeight = mIni.getInteger("WINDOW", "iHeight", mWindowHeight);

	bool fullscreen = mIni.getBoolean("WINDOW", "bFullscreen", false);
	bool borderless = mIni.getBoolean("WINDOW", "bBorderless", false);
	bool resizeable = mIni.getBoolean("WINDOW", "bResizeable", false);

	if (fullscreen)
		mWindowStates |= SDL_WINDOW_FULLSCREEN;
	if (borderless)
		mWindowStates |= SDL_WINDOW_BORDERLESS;
	if (resizeable)
		mWindowStates |= SDL_WINDOW_RESIZABLE;

	bool vsync = mIni.getBoolean("RENDERER", "bVsync", false);
	bool software = mIni.getBoolean("RENDERER", "bSoftware", false);
	mFramelock = mIni.getBoolean("RENDERER", "bFramelock", mFramelock);

	if (vsync)
		mRenderingContext |= SDL_RENDERER_PRESENTVSYNC;
	if (software)
		mRenderingContext |= SDL_RENDERER_SOFTWARE;

	mDebug = mIni.getBoolean("ENGINE", "bDebug", mDebug);
}
