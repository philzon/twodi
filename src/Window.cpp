#include "Window.hpp"

Window::Window()
{
	mIsOpen = true;

	mWindow = nullptr;
	mRenderer = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);

	mWindow = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_POSITION,
		WINDOW_POSITION,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_STATES);

	mRenderer = SDL_CreateRenderer(mWindow,
		RENDERER_INDEX,
		RENDERER_CONTEXT);

	mCamera.x = 0;
	mCamera.y = 0;
}

Window::~Window()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);

	SDL_Quit();
}

void Window::draw(const Drawable &drawable)
{
	drawable.draw(mWindow, mRenderer, mCamera);
}

void Window::close()
{
	mIsOpen = false;
}

void Window::clear()
{
	clear(0, 0, 0);
}

void Window::clear(char red, char green, char blue)
{
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	SDL_RenderFillRect(mRenderer, NULL);
}

void Window::display()
{
	SDL_RenderPresent(mRenderer);
}

////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////

Camera Window::getCamera() const
{
	return Camera(mCamera.x, mCamera.y);
}

bool Window::isOpen() const
{
	return mIsOpen;
}

int Window::getWidth() const
{
	int width = 0;
	SDL_GetWindowSize(mWindow, &width, NULL);
	return width;
}

int Window::getHeight() const
{
	int height = 0;
	SDL_GetWindowSize(mWindow, NULL, &height);
	return height;
}

////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////

void Window::setCamera(const Camera &camera)
{
	mCamera.x = static_cast<int>(camera.getLeft());
	mCamera.y = static_cast<int>(camera.getTop());
}

void Window::setFullscreen(bool state, bool desktop)
{
	int mode = SDL_WINDOW_FULLSCREEN;

	// Set to fake fullscreen.
	if (desktop)
		mode = SDL_WINDOW_FULLSCREEN_DESKTOP;

	// Set to windowed mode.
	if (!state)
		mode = 0;

	SDL_SetWindowFullscreen(mWindow, mode);
}

void Window::setResizeable(bool state)
{
	if (state)
		SDL_SetWindowResizable(mWindow, SDL_TRUE);
	else
		SDL_SetWindowResizable(mWindow, SDL_FALSE);
}

void Window::setSize(int width, int height)
{
	SDL_SetWindowSize(mWindow, width, height);
}

void Window::setTitle(const std::string &title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

void Window::setVsync(bool state)
{
	if (state)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
}
