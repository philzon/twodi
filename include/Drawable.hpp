#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <SDL2/SDL.h>

class Drawable
{
public:
	Drawable();
	virtual ~Drawable();

	virtual void draw(SDL_Window *window, SDL_Renderer *renderer) const = 0;
};

#endif // DRAWABLE_HPP
