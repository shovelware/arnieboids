#include <include\Camera.hpp>

Camera::Camera(sf::RenderTarget& target) :
	rentrg_(target),
	screenSize_(target.getSize()),
	View(sf::Vector2f(target.getSize().x / 2, target.getSize().y / 2), sf::Vector2f(target.getSize())),
	radarSize_(128),
	radarPos_(screenSize_.x - radarSize_, radarSize_),
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

		text_ = sf::Text("", *font_);
		text_.setCharacterSize(25);
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

void Camera::drawHUD()
{
	rentrg_.setView(rentrg_.getDefaultView());

	bool fnt = (font_ != nullptr);
	bool trg = (target_ != nullptr);
	

	//Draw general text items
	if (fnt)
	{
		//Zoom
		drawText(std::string("Z: ") + std::to_string((int)getZoomPercent()),
			sf::Vector2f(0, 58),
			sf::Color::White
			);

		//Take camera pos, might add target pos later
		sf::Vector2f pos = move_;

		if (trg)
		{
			//Add target to pos if we have one
			pos += target_->getPosition();
			
			//Draw controlled text items
			//Radar range
			//text_.setString(std::string(std::to_string(target_->getRadarRange()) + "sm"));

			//Health bar
			//Fire capabilities bar
			//Accel bar
			
		}

		//Pos
		drawText(std::string("X: ") + std::to_string(pos.x) + std::string("\nY: ") + std::to_string(pos.y),
			sf::Vector2f(0, 0),
			sf::Color::White);
	}

	rentrg_.setView(*this);
}

void Camera::drawRadar(std::list<Ship*>& objList, sf::Vector2f centre, float radius)
{
	rentrg_.setView(rentrg_.getDefaultView());

	if (target_ != nullptr)
	{
		//Background
		sf::CircleShape radarBG(radarSize_);
		radarBG.setFillColor(sf::Color(128, 128, 128, 64));
		radarBG.setOutlineThickness(-1);
		radarBG.setOutlineColor(sf::Color(255, 255, 255, 64));

		radarBG.setPosition(radarPos_);
		radarBG.setOrigin(sf::Vector2f(radarSize_, radarSize_));
		rentrg_.draw(radarBG);

		radarBG.setRadius(radarSize_ * 3/4);
		radarBG.setOrigin(sf::Vector2f(radarBG.getRadius(), radarBG.getRadius()));
		rentrg_.draw(radarBG);
		
		radarBG.setRadius(radarSize_ / 2);		
		radarBG.setOrigin(sf::Vector2f(radarBG.getRadius(), radarBG.getRadius()));
		rentrg_.draw(radarBG);
		
		radarBG.setRadius(radarSize_ * 1/4);
		radarBG.setOrigin(sf::Vector2f(radarBG.getRadius(), radarBG.getRadius()));
		rentrg_.draw(radarBG);

		//Radar always draws transparent in the top 200*200 of the screen
		//Radar is scaled to keep range in 100 and ships are scaled down
		for (Ship* s : objList)
		{
			if (s != target_)
			{
				if (thor::length(s->getPosition() - target_->getPosition()) < target_->getRadarRange())
				{
					drawRadarObj(s);
				}
			}
		}

		drawRadarObj(target_);
	}



	rentrg_.setView(*this);
}

float Camera::getZoomPercent() const
{
	return 200 - (zoomFactor_ * 100);
}

void Camera::drawText(std::string info, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Color color = sf::Color::White)
{
	//Check if hud font and text are initialised
	if (font_ != nullptr)
	{
		text_.setString(info);
		text_.setPosition(pos);
		text_.setColor(color);
		rentrg_.draw(text_);
	}
}

void Camera::drawRadarObj(sf::ConvexShape * shape)
{
	if (target_ != nullptr)
	{
		int shipSize = 2;

		sf::CircleShape shipShape(shipSize);

		shipShape.setOrigin(sf::Vector2f(shipSize, shipSize));
		shipShape.setPosition(radarPos_ + worldToRadar(shape->getPosition()));

		sf::Color alphaFill(shape->getFillColor());
		alphaFill.a = 128;
		shipShape.setFillColor(alphaFill);

		sf::Color alphaOutline(shape->getOutlineColor());
		alphaOutline.a = 128;
		shipShape.setOutlineColor(alphaOutline);

		shipShape.setOutlineThickness(-1);

		rentrg_.draw(shipShape);
	}
}

sf::Vector2f Camera::worldToRadar(sf::Vector2f const& worldPos)
{
	if (target_)
	{
		/*	// Complete this function so that it returns normalized coordinates (coordinates in the interval [0, 1]) for pixel coordinates (x, y).
		pair<float, float> f1(unsigned int x, unsigned int y, unsigned int screenWidth, unsigned int screenHeight)
		{
			// = minimum normalised range + (total normalised range / total pixel range) * pixel coordinate)
			//i.e. interval of [-1,1] is = -1 + (2 / screenWidth) * x;
			float newX = 0 + (1.0 / (screenWidth > 0 ? screenWidth : 1)) * x;
			float newY = 0 + (1.0 / (screenHeight > 0 ? screenHeight : 1)) * y;

			return make_pair(newX, newY);
		}*/

		//takes a world pos (-size <-> size)
		//and converts it into radarPos (-radarSize <-> radarSize)

		float range = target_->getRadarRange();

		sf::Vector2f radarPos(0, 0);

		radarPos.x = (radarSize_ / screenSize_.x) * (worldPos.x - target_->getPosition().x);
		radarPos.y = (radarSize_ / screenSize_.y) * (worldPos.y - target_->getPosition().y);

		return radarPos;
	}
}
