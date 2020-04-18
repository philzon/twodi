#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL2/SDL.h"

#include "Camera.hpp"
#include "Drawable.hpp"
#include "Window.hpp"

// Renderer default constants.
#define RENDERER_INDEX    -1
#define RENDERER_CONTEXT  SDL_RENDERER_ACCELERATED

class Renderer
{
public:
	Renderer(const Window &window);
	virtual ~Renderer();

	void draw(const Drawable &drawable);

	void clear();
	void clear(char red, char green, char blue);
	void display();

	// Getters.
	Camera getCamera() const;

	// Setters.
	void setCamera(const Camera &camera);
	void setVsync(bool state);

private:
	Renderer(const Renderer &other) = delete;
	Renderer &operator=(const Renderer &other) = delete;

	Camera mCamera;
	SDL_Renderer *mRenderer;
};

#endif // RENDERER_HPP
