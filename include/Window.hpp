#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include <SDL2/SDL.h>

// Window default constants.
#define WINDOW_TITLE     ""
#define WINDOW_WIDTH     800
#define WINDOW_HEIGHT    600
#define WINDOW_POSITION  SDL_WINDOWPOS_CENTERED
#define WINDOW_STATES    SDL_WINDOW_SHOWN

class Window
{
public:
	friend class Renderer;

	Window();
	virtual ~Window();

	void close();

	// Getters.
	int getWidth() const;
	int getHeight() const;
	bool isOpen() const;

	// Setters.
	void setFullscreen(bool state, bool desktop);
	void setResizeable(bool state);
	void setSize(int width, int height);
	void setTitle(const std::string &title);

private:
	Window(const Window &other) = delete;
	Window &operator=(const Window &other) = delete;

	bool mIsOpen;
	SDL_Window *mWindow;
};

#endif // WINDOW_HPP
