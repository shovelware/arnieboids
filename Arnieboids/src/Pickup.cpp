#include <include\Pickup.hpp>

Pickup::Pickup(sf::Vector2f const & position, int value) :
owner_(nullptr),
value_(value),
ticks_(0),
shieldTimeTotal_(value_ * 2),
shieldTime_(0)
{
	setPointCount(8u);

	setShape(16);
	
	setFillColor(sf::Color::Blue);
	setOutlineThickness(-1.f);
	setOutlineColor(sf::Color::White);

	setPosition(position);
}

void Pickup::update()
{
	ticks_ = (ticks_ + 1) % INT_MAX;

	if (owner_ == nullptr)
	{
		if (value_ <= 0)
		{

		}

		//If we have no owner update our colours every second
		int s = static_cast<int>(tickToSec(ticks_));

		if ( s % 2)
		{
			setOutlineColor(sf::Color::Cyan);
		}

		else setOutlineColor(sf::Color::White);

		rotate(1.f);
	}

	//Else if we do, count down our time
	else
	{
		//If we're out of time
		if (shieldTime_ < shieldTimeTotal_)
		{
			shieldTime_ = tickToSec(ticks_ - takeTick_);
		}

		//Otherwise just follow our owner
		setPosition(owner_->getPosition());
		setRotation(owner_->getRotation());
	}
}

void Pickup::take(Ship * taker)
{
	owner_ = taker;

	//If taker is hurt, heal it
	int healthDiff = owner_->getMaxHealth() - owner_->getHealth();

	if (healthDiff > 0)
	{
		if (value_ > 0)
		{
			owner_->addHealth(value_);
			value_ = (value_ - healthDiff >= 0 && value_ - healthDiff < 100? value_ - healthDiff : 0);

			//Update shields
			shieldTimeTotal_ = value_ * 2;
		}
		owner_ = nullptr;
	}

	//Otherwise, convert to a shield for them
	else if (healthDiff <= 0)
	{
		takeTick_ = ticks_;

		sf::Color fill(owner_->getFillColor());
		sf::Color out(owner_->getOutlineColor());

		fill.a = 128;
		out.a = 128;

		setFillColor(fill);
		setOutlineColor(out);

		int w = owner_->getGlobalBounds().width;
		int h = owner_->getGlobalBounds().height;
		
		value_ = 0;

		//Set shape to radius of larger dimension
		setShape((w > h ? w : h));
	}
}

bool Pickup::isActive()
{
	return (value_ != 0 || shieldTime_ < shieldTimeTotal_);
}

void Pickup::setShape(float radius)
{
	sf::Vector2f point(0, 0);

		for (unsigned int i = 0; i < 8 ; ++i)
		{
			point.x = radius * (cos(thor::Pi / 180 * 360 / 8 * i));
			point.y = radius * (sin(thor::Pi / 180 * 360 / 8 * i));

			setPoint(i, point);
		}

		setOrigin(sf::Vector2f(0, 0));
}

bool Pickup::isOwned() const
{
	return (owner_ != nullptr);
}

float Pickup::tickToSec(unsigned int ticks) const
{
	return (16.f / 1000.f) * ticks;
}

