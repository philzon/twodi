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

	mFramelock = true;
	mDebug = false;

	configure("config.ini");
	parse(args);

	mWindow.setTitle(BUILD_TITLE);

	mLog->info("");
	mLog->info("== Window ==================");
	mLog->info("   Fullscreen  " + mIni.getString("WINDOW", "bFullscreen", "false"));
	mLog->info("   Borderless  " + mIni.getString("WINDOW", "bBorderless", "false"));
	mLog->info("   Resizeable  " + mIni.getString("WINDOW", "bResizeable", "false"));
	mLog->info("   Width       " + mIni.getString("WINDOW", "iWidth", "800"));
	mLog->info("   height      " + mIni.getString("WINDOW", "iHeight", "600"));
	mLog->info("== Renderer ================");
	mLog->info("   Vsync       " + mIni.getString("RENDERER", "bVsync", "false"));
	mLog->info("   Software    " + mIni.getString("RENDERER", "bSoftware", "false"));
	mLog->info("   Framelock   " + mIni.getString("RENDERER", "bFramelock", "false"));
	mLog->info("== Engine ==================");
	mLog->info("   Debug       " + mIni.getString("ENGINE", "bDebug", "false"));
	mLog->info("");
}

Engine::~Engine()
{}

int Engine::run()
{
	unsigned int last = 0;
	double unprocessed = 0.0f;

	while (mWindow.isOpen())
	{
		unprocessed += (SDL_GetTicks() - last) / (1000.0f / ENGINE_TPS);
		last = SDL_GetTicks();

		while (SDL_PollEvent(&mEvent))
		{
			switch (mEvent.type)
			{
			case SDL_QUIT:
				mWindow.close();
				mLog->info("QUIT event triggered");
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
			mWindow.clear();

			// Perform graphics operation here ...

			mWindow.display();

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
					+ std::to_string(mWindow.getWidth()) \
					+ std::string("x") \
					+ std::to_string(mWindow.getHeight()) \
					+ std::string(" | ") \
					+ std::to_string(mFPS) \
					+ std::string(" FPS | ") \
					+ std::to_string(mTPS) \
					+ std::string(" TPS | ") \
					+ std::to_string(mTick) \
					+ std::string(" Tick");

				mWindow.setTitle(title);
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

	int width = mIni.getInteger("WINDOW", "iWidth", 800);
	int height = mIni.getInteger("WINDOW", "iHeight", 600);
	bool fullscreen = mIni.getBoolean("WINDOW", "bFullscreen", false);
	bool borderless = mIni.getBoolean("WINDOW", "bBorderless", false);
	bool resizeable = mIni.getBoolean("WINDOW", "bResizeable", false);

	bool vsync = mIni.getBoolean("RENDERER", "bVsync", false);
	// bool software = mIni.getBoolean("RENDERER", "bSoftware", false);
	mFramelock = mIni.getBoolean("RENDERER", "bFramelock", mFramelock);

	mDebug = mIni.getBoolean("ENGINE", "bDebug", mDebug);

	mWindow.setSize(width, height);
	mWindow.setFullscreen(fullscreen, borderless);
	mWindow.setResizeable(resizeable);
	mWindow.setVsync(vsync);
}
