#include "Camera.hpp"

Camera::Camera()
{
	mLeft = 0;
	mTop = 0;
}

Camera::Camera(float left, float top)
{
	mLeft = left;
	mTop = top;
}

Camera::Camera(const Camera &other)
{
	mLeft = other.mLeft;
	mTop = other.mTop;
}

Camera &Camera::operator=(const Camera &other)
{
	if (this != &other)
	{
		mLeft = other.mLeft;
		mTop = other.mTop;
	}

	return *this;
}

Camera::~Camera()
{}

////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////

float Camera::getLeft() const
{
	return mLeft;
}

float Camera::getTop() const
{
	return mTop;
}

////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////

void Camera::setPosition(float left, float top)
{
	mLeft = left;
	mTop = top;
}
