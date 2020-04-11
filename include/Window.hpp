#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

#include <SDL2/SDL.h>

#include "Drawable.hpp"
#include "Camera.hpp"

// Window default constants.
#define WINDOW_TITLE      ""
#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600
#define WINDOW_POSITION   SDL_WINDOWPOS_CENTERED
#define WINDOW_STATES     SDL_WINDOW_SHOWN

// Renderer default constants.
#define RENDERER_INDEX    -1
#define RENDERER_CONTEXT  SDL_RENDERER_ACCELERATED

class Window
{
public:
	Window();
	virtual ~Window();

	void draw(const Drawable &drawable);

	void close();
	void clear();
	void clear(char red, char green, char blue);
	void display();

	// Getters.
	Camera getCamera() const;
	int getWidth() const;
	int getHeight() const;
	bool isOpen() const;

	// Setters.
	void setCamera(const Camera &camera);
	void setFullscreen(bool state, bool desktop);
	void setResizeable(bool state);
	void setSize(int width, int height);
	void setTitle(const std::string &title);
	void setVsync(bool state);

private:
	Window(const Window &other) = delete;
	Window &operator=(const Window &other) = delete;

	bool mIsOpen;

	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	SDL_Point mCamera;
};

#endif // WINDOW_HPP
