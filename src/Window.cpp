#include "Window.hpp"

Window::Window()
{
	mIsOpen = true;

	mWindow = SDL_CreateWindow(WINDOW_TITLE,
		WINDOW_POSITION,
		WINDOW_POSITION,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_STATES);
}

Window::~Window()
{
	SDL_DestroyWindow(mWindow);
}

void Window::close()
{
	mIsOpen = false;
}

////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////

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
