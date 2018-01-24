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

	if (fnt->loadFromFile("./" + filename));
	{
		//If we load successfully, assign font
		font_ = fnt;

		text_ = sf::Text("", *font_);
		text_.setCharacterSize(50);
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
			drawText(std::string("H: ") + std::to_string(target_->getHealth()) + std::string("/") + std::to_string(target_->getMaxHealth()),
				sf::Vector2f(0, screenSize_.y - 60),
				sf::Color::Red);
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


		//Radar always draws transparent in the top 128*128 of the screen
		//Radar is scaled to keep range in 100 and ships are scaled down
		for (Ship* s : objList)
		{
			if (s != target_)
			{
				//If the object is within radar range
				//if (thor::length(target_->getPosition() - s->getPosition()) <= target_->getRadarRange())
				{
					drawRadarObj(s);
				}
			}
		}

		radarBG.setRadius(2);
		radarBG.setOrigin(sf::Vector2f(radarBG.getRadius(), radarBG.getRadius()));
		radarBG.setFillColor(target_->getFillColor());
		radarBG.setOutlineColor(target_->getOutlineColor());
		rentrg_.draw(radarBG);
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
		text_.setFillColor(color);
		rentrg_.draw(text_);
	}
}

void Camera::drawRadarObj(sf::ConvexShape * shape)
{
	if (target_ != nullptr)
	{
		//Get radar position
		sf::Vector2f targetShipPos(worldToRadar(target_->getPosition()));
		sf::Vector2f radarShipPos(worldToRadar(shape->getPosition()));
		float radarDist = thor::length(radarShipPos - targetShipPos);
			
		float maxSize = radarSize_ / 20.f;
		float genSize = (radarDist  / 10.f);
		float shipSize = ((genSize < maxSize && genSize != 0)? genSize : maxSize);
		//If it's within the circle

		if (radarDist < radarSize_ - genSize)
		{
			//Figure size based on distance
			//shipSize = 4;

			//Create shape
			sf::CircleShape shipShape(genSize);

			//Set origin and position
			shipShape.setOrigin(sf::Vector2f(shipShape.getRadius(), shipShape.getRadius()));
			shipShape.setPosition(radarPos_ + (radarShipPos - targetShipPos));

			//Set fill
			sf::Color alphaFill(shape->getFillColor());
			alphaFill.a = 128;
			shipShape.setFillColor(alphaFill);

			//Set outline
			sf::Color alphaOutline(shape->getOutlineColor());
			alphaOutline.a = 128;
			shipShape.setOutlineColor(alphaOutline);
			shipShape.setOutlineThickness(-1);

			rentrg_.draw(shipShape);
		}
	}
}

sf::Vector2f Camera::worldToRadar(sf::Vector2f const& worldPos)
{
	if (target_)
	{
		//Translates from world space to range space
		//Then from range to radar display
		//Returns co-ords of vector relative to radar display centre
		float range = target_->getRadarRange();		

		sf::FloatRect worldSpace(-screenSize_.x * 1.5f, -screenSize_.y * 1.5f, screenSize_.x * 3.f, screenSize_.y * 3.f);
		sf::FloatRect rangeSpace(-range / 2, -range / 2, range / 2, range / 2);
		sf::FloatRect displaySpace(-radarSize_, -radarSize_, radarSize_, radarSize_);

		sf::Vector2f radarRangePos(translateVector(worldPos, worldSpace, rangeSpace));

		sf::Vector2f radarPos(translateVector(radarRangePos, rangeSpace, displaySpace));

		return radarPos;
	}
}
	sf::Vector2f Camera::translateVector(sf::Vector2f pos, sf::FloatRect src, sf::FloatRect dest)
	{
		sf::Vector2f result(0, 0);

		//res = (src - src_min) / (src_max - src_min) * (res_max - res_min) + res_min
		result.x = (pos.x - src.left) / (src.width - src.left) * (dest.width - dest.left) + dest.left;
		result.y = (pos.y - src.top) / (src.height - src.top) * (dest.height - dest.top) + dest.top;

		return result;
	}
