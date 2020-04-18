#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class Renderer;
class Drawable
{
public:
	friend class Renderer;

	Drawable();
	virtual ~Drawable();

	virtual void draw(const Renderer &renderer) const = 0;
};

#endif // DRAWABLE_HPP
