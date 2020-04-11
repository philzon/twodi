#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera
{
public:
	Camera();
	Camera(float left, float top);
	Camera(const Camera &other);
	Camera &operator=(const Camera &other);
	virtual ~Camera();

	// Getters.
	float getLeft() const;
	float getTop() const;

	// Setters.
	void setPosition(float left, float top);

private:
	float mLeft;
	float mTop;
};

#endif // CAMERA_HPP
