#include <include\Camera.hpp>

Camera::Camera(sf::Vector2u screenSize) : 
View(sf::Vector2f(screenSize.x / 2, screenSize.y / 2), sf::Vector2f(screenSize)),
screenSize_(screenSize),
zoomFactor_(1),
zoomStep_(0.05f),
font_(nullptr)
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

void Camera::loadFont(std::string filename)
{
	sf::Font* fnt = new sf::Font();

	if (fnt->loadFromFile("./font/" + filename));
	{
		//If we load successfully, assign font
		font_ = fnt;
	}
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

void Camera::drawHUD(sf::RenderTarget& rentrg)
{
	rentrg.setView(rentrg.getDefaultView());

	bool fnt = (font_ != nullptr);
	bool trg = (target_ != nullptr);

	if (fnt)
	{
		
		//Draw general text items
		//Zoom
		sf::Text zoomText = sf::Text(std::string("Zoom: ") + std::to_string((int)getZoomPercent()), *font_);
		zoomText.setPosition(0, 0);
		rentrg.draw(zoomText);
		//Pos
		
		if (trg)
		{
			//Draw controlled text items
			//Radar range

			//Health bar
			//Fire capabilities bar
			//Accel bar
			
			drawRadar(rentrg, target_->getPosition(), target_->getRadarRange());
		}
	}

	rentrg.setView(*this);
}

void Camera::drawRadar(sf::RenderTarget& rentrg, sf::Vector2f centre, float radius)
{
}

float Camera::getZoomPercent() const
{
	return 200 - (zoomFactor_ * 100);
}
