#include <include/Ship.hpp>

Ship::Ship(sf::Vector2f const &position, float maxSpeed, unsigned int health) :
MAX_SPEED_(maxSpeed),
velocity_(0.f, 0.f),
health_(health),
forward_(0.f, -1.f),
turnSpeed_(2.f),
thrust_(0.1f),
refireTime_(2.f),
coolDown_(0)
{
	setPosition(position);
}

Ship::~Ship() {
	
}

void Ship::takeDamage(unsigned amount) {
	if (amount <= health_)
	{
		health_ -= amount;
	}
	else
	{
		health_ = 0U;
	}
}

bool Ship::isDead() const {
	return health_ == 0;
}

void Ship::clampToMaxSpeed() {
	if (thor::length(velocity_) > MAX_SPEED_)
	{
		velocity_ = thor::unitVector(velocity_);
		velocity_ *= MAX_SPEED_;
	}
}

void Ship::thrust() {

	//Calculate and pplay delta velocity
	velocity_.x += forward_.x * thrust_;
	velocity_.y += forward_.y * thrust_;

	clampToMaxSpeed();
}

void Ship::turnLeft()
{
	//Get rotation in radians
	float rotRads = thor::toRadian(getRotation());

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);

	rotate(-turnSpeed_);
}

void Ship::turnRight()
{
	//Get rotation in radians
	float rotRads = thor::toRadian(getRotation());

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);

	rotate(turnSpeed_);
}

//Should this be here?
bool Ship::trigger()
{
	bool fired = false;
	
	//Handle firing logic, return success here
	if (coolDown_ <= 0)
	{
		coolDown_ = refireTime_;
		fired = true;
	}

	return fired;
}

sf::Vector2f Ship::getForward() const
{
	return forward_;
}

float Ship::tickToSec(unsigned int ticks) const
{
	return (16.f / 1000.f) * ticks;
}