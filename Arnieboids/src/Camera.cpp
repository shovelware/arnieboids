#include <include\Camera.hpp>

Camera::Camera(sf::Vector2u screenSize) : 
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
screenSize_(screenSize),
zoomFactor_(1),
zoomStep_(0.05f)
{

}

Camera::Camera(sf::Vector2u screenSize, Ship * target) :
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
screenSize_(screenSize),
target_(target),
zoomFactor_(1),
zoomStep_(0.05f)
{
}

void Camera::setTarget(Ship * target)
{
	target_ = target;
}

Ship * Camera::getTarget()
{
	return target_;
}

void Camera::clearTarget(bool savexf)
{
	if (savexf)
	{
		move_ += target_->getPosition();
	}

	target_ = nullptr;
}

void Camera::update()
{
	if (target_ != nullptr)
	{
		setCenter(target_->getPosition() + move_);
	}

	else setCenter(move_);
}

void Camera::move(sf::Vector2f xf)
{
	move_ += xf;
}

void Camera::moveReset()
{
	move_ = sf::Vector2f(0, 0);
}

void Camera::zoomIn()
{
	zoomFactor_ -= zoomStep_;
	zoom(1 - zoomStep_);
}

void Camera::zoomOut()
{
	zoomFactor_ += zoomStep_;
	zoom(1 + zoomStep_);
}

void Camera::zoomSet(float mult)
{
	zoom(mult);
}

void Camera::zoomReset()
{
	zoomFactor_ = 1;
	reset(sf::FloatRect(sf::Vector2f(0, 0), screenSize_));
}

float Camera::getZoomPercent() const
{
	return zoomFactor_ * 100;
}
