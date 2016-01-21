#include <include/Player.hpp>
#include <iostream>
#include <include/Asteroid.hpp>

Player::Player(thor::ParticleSystem &particleSystem, sf::Vector2f const& position, unsigned int maxHealth) :
Ship(particleSystem, position, 4.f, maxHealth)
{
	setPointCount(3u);

	// Shape:
	//	   / \
	//	  /	  \
	//	 /	   \
	//	/_______\

	setPoint(0u, sf::Vector2f(10, 0));
	setPoint(1u, sf::Vector2f(20, 30));
	setPoint(2u, sf::Vector2f(0, 30));

	setOrigin(getLocalBounds().width * 0.5f, getLocalBounds().height * 0.5f);

	setFillColor(sf::Color::Blue);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);

	refireTime_ = .5f;

	particleEmitter_.setParticleColor(getOutlineColor());
	particleAngleVariance_ = 35.f;
}

Player::~Player() {
	
}

void Player::update() {
	Ship::update();
	move(velocity_);

	ticks_ = (ticks_ + 1) % INT_MAX;

	//Cool weapon
	coolDown_ -= tickToSec(1);
}

void Player::onCollide(Ship* other) {
	if (!dynamic_cast<Asteroid*>(other))
		takeDamage();
}
