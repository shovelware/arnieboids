#include <include/Ship.hpp>

Ship::Ship(float maxSpeed, unsigned int health) :
MAX_SPEED_(maxSpeed),
velocity_(0.f, 0.f),
health_(health),
forward_(0.f, 0.f),
turnSpeed_(1.f),
thrust_(0.1f)
{
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
	//Get rotation in radians
	float rotRads = getRotation() * 0.017f;

	//Calculate forward vector
	forward_.y = -cosf(rotRads);
	forward_.x = sinf(rotRads);

	//Calculate and pplay delta velocity
	velocity_.x += forward_.x * thrust_;
	velocity_.y += forward_.y * thrust_;

	clampToMaxSpeed();
}
