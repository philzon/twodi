#include "Engine.hpp"

Engine::Engine(const std::vector<std::string> &args)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	mLog = Log::getInstance();
	mLog->info("Starting application: "
		+ std::string(BUILD_TITLE)
		+ " "
		+ std::string(BUILD_VERSION)
		+ " ("
		+ std::string(BUILD_COMMIT)
		+ ")");
	mLog->info("");

	mFPS = 0;
	mTPS = 0;
	mTick = 0;

	if (mIni.load(CONFIG_PATH))
		mLog->info("Loaded " + std::string(CONFIG_PATH));
	else
		mLog->info("Could not load " + std::string(CONFIG_PATH));

	if (!mIni.getErrors().empty())
	{
		mLog->warning("Config contains errors: " + std::string(CONFIG_PATH));
		mLog->warning(mIni.getErrors());
	}

	mFramelock = mIni.getBoolean("ENGINE", "bFramelock", true);
	mDebug = mIni.getBoolean("ENGINE", "bDebug", false);

	parse(args);
}

Engine::~Engine()
{
	SDL_Quit();
}

int Engine::run()
{
	Window window;
	window.setTitle(BUILD_TITLE);
	window.setSize(mIni.getInteger("WINDOW", "iWidth", 800),
		mIni.getInteger("WINDOW", "iHeight", 600));
	window.setFullscreen(mIni.getBoolean("WINDOW", "bFullscreen", false),
		mIni.getBoolean("WINDOW", "bBorderless", false));
	window.setResizeable(mIni.getBoolean("WINDOW", "bResizeable", false));

	Renderer renderer(window);
	renderer.setVsync(mIni.getBoolean("RENDERER", "bVsync", false));

	SDL_Event event;

	unsigned int last = 0;
	double unprocessed = 0.0f;

	while (window.isOpen())
	{
		unprocessed += (SDL_GetTicks() - last) / (1000.0f / ENGINE_TPS);
		last = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				window.close();
				mLog->info("QUIT event triggered");
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					mLog->info("Window resized to "
						+ std::to_string(event.window.data1)
						+ "x"
						+ std::to_string(event.window.data2));
					break;

				}
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
			renderer.clear();

			// Perform graphics operation here ...

			renderer.display();

			++mFPS;
		}

		// Update loop information each second.
		if (shouldUpdate && mTick % 60 == 0)
		{
			// Refresh debug title each second.
			if (mDebug)
			{
				std::string title = std::string(BUILD_TITLE) \
					+ std::string(" ") \
					+ std::string(BUILD_VERSION) \
					+ std::string("-") \
					+ std::string(BUILD_BUILD) \
					+ std::string(" (") \
					+ std::string(BUILD_COMMIT) \
					+ std::string(" -> ") \
					+ std::string(BUILD_BRANCH) \
					+ std::string(") | ") \
					+ std::to_string(window.getWidth()) \
					+ std::string("x") \
					+ std::to_string(window.getHeight()) \
					+ std::string(" | ") \
					+ std::to_string(mFPS) \
					+ std::string(" FPS | ") \
					+ std::to_string(mTPS) \
					+ std::string(" TPS | ") \
					+ std::to_string(mTick) \
					+ std::string(" Tick");

				window.setTitle(title);
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
