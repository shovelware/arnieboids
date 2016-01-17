#include <include/Player.hpp>
#include <iostream>

Player::Player(thor::ParticleSystem &particleSystem, sf::Vector2f const& position, unsigned int maxHealth) :
Ship(particleSystem, position, 4.f),
MAX_HEALTH_(maxHealth)
{
	setPointCount(3u);

	// Shape:
	//	   / \
	//	  /	  \
	//	 /	   \
	//	/_______\

	setPoint(0u, sf::Vector2f(0, -15));
	setPoint(1u, sf::Vector2f(10, 15));
	setPoint(2u, sf::Vector2f(-10, 15));

	setFillColor(sf::Color::Blue);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);

	refireTime_ = .5f;

	particleEmitter_.setParticleColor(getOutlineColor());
}

Player::~Player() {
	
}

void Player::update() {
	move(velocity_);

	ticks_ = (ticks_ + 1) % INT_MAX;

	//Cool weapon
	coolDown_ -= tickToSec(1);

	updateParticleEmitter();
}

void Player::onCollide(Ship* other) {
}