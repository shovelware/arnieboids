#include <include\Camera.hpp>

Camera::Camera(sf::Vector2u screenSize) : 
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
screenSize_(screenSize)
{

}

Camera::Camera(sf::Vector2u screenSize, Ship * target) :
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
screenSize_(screenSize),
target_(target)
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

void Camera::clearTarget()
{
	target_ = nullptr;
}

void Camera::update()
{
	if (target_ != nullptr)
	{
		setCenter(target_->getPosition());
	}
}
