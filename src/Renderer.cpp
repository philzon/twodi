#include "Renderer.hpp"

Renderer::Renderer(const Window &window, bool software)
{
	int flags = RENDERER_CONTEXT;

	if (software)
		flags |= SDL_RENDERER_SOFTWARE;

	mRenderer = SDL_CreateRenderer(window.mWindow,
		RENDERER_INDEX,
		flags);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
}

void Renderer::clear()
{
	clear(0, 0, 0);
}

void Renderer::clear(char red, char green, char blue)
{
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, red, green, blue, 255);
	SDL_RenderFillRect(mRenderer, NULL);
}

void Renderer::display()
{
	SDL_RenderPresent(mRenderer);
}

void Renderer::draw(const Drawable &drawable)
{
	drawable.draw(*this);
}


////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////

Camera Renderer::getCamera() const
{
	return mCamera;
}

////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////

void Renderer::setCamera(const Camera &camera)
{
	mCamera = camera;
}


void Renderer::setVsync(bool state)
{
	if (state)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
}
