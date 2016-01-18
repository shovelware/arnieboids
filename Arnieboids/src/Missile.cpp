#include <include/Missile.hpp>

Missile::Missile(thor::ParticleSystem& particleSystem, const Ship* const target, sf::Vector2f const& position, sf::Vector2f const& direction, const float maxSpeed, const float acceleration, const float turnSpeed) :
Bullet(position, direction, 0.f),
TARGET_(target),
MAX_SPEED_(maxSpeed),
ACCELERATION_(acceleration),
TURN_SPEED_(turnSpeed),
isAccelerating_(true)
{
	setPointCount(5u);

	setPoint(0u, sf::Vector2f(0, 5));
	setPoint(1u, sf::Vector2f(0, 20));
	setPoint(2u, sf::Vector2f(6, 20));
	setPoint(3u, sf::Vector2f(6, 5));
	setPoint(4u, sf::Vector2f(3, 0));

	setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setFillColor(sf::Color(100, 0, 0));

	setOutlineColor(sf::Color::Red);

	setRotation(thor::toDegree(atan2(forward_.y, forward_.x)));	//get forward vector as angle in degrees

	particleEmitter_.setParticleColor(getOutlineColor());
	particleEmitter_.setParticleScale(sf::Vector2f(0.1f, 0.1f));
	particleEmitter_.setEmissionRate(30.f);
	particleEmitter_.setParticleLifetime(sf::seconds(0.3f));
	connection_ = particleSystem.addEmitter(thor::refEmitter(particleEmitter_));

	lifeTime_ = 10;
}

Missile::~Missile() {
	//disconnect emitter from particle system
	connection_.disconnect();
}

// returns sin(theta)*length(a)*length(b), where theta = the angle between vectors a,b
inline float perpDot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.y*b.x) - (a.x*b.y);
}

void Missile::update() {

	if (active_)
	{
		//find out which way we should turn to face target
		float d = perpDot(TARGET_->getPosition() - this->getPosition(), forward_);
		if (d > 0.f) {
			//turn clockwise
			rotate(TURN_SPEED_);
		}
		else {
			//turn counter-clockwise
			rotate(-TURN_SPEED_);
		}

		//accelerate to top speed
		if (isAccelerating_) {
			speed_ += ACCELERATION_;
			if (speed_ > MAX_SPEED_) {
				speed_ = MAX_SPEED_;
				isAccelerating_ = false;
			}
		}


		//Get rotation in radians
		float rotRads = getRotation() * 0.017f;

		//Calculate forward vector
		forward_.y = -cosf(rotRads);
		forward_.x = sinf(rotRads);

		move(forward_ * speed_);


		ticks_ = (ticks_ + 1) % INT_MAX;

		if (tickToSec(ticks_) >= lifeTime_)
		{
			active_ = false;
		}

		updateParticleEmitter();
	}
}

void Missile::updateParticleEmitter() {
	particleEmitter_.setParticlePosition(getPosition() + (-forward_ * getLocalBounds().height * 0.5f));
	particleEmitter_.setParticleRotation(getRotation());
	//particleEmitter_.setParticleVelocity(-forward_ * speed_);
}
