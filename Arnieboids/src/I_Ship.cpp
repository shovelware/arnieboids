#include <include/I_Ship.hpp>

I_Ship::I_Ship(float maxSpeed) :
MAX_SPEED_(maxSpeed),
velocity_(0.f, 0.f)
{
}

I_Ship::~I_Ship() {
	
}

void I_Ship::clampToMaxSpeed() {
	if (thor::length(velocity_) > MAX_SPEED_)
	{
		velocity_ = thor::unitVector(velocity_);
		velocity_ *= MAX_SPEED_;
	}
}
