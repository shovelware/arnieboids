#include <include/Player.hpp>
#include <iostream>

Player::Player(sf::Vector2f const& position, unsigned int maxHealth) :
Ship(position, 4.f),
MAX_HEALTH_(maxHealth)
{
	setPointCount(3u);

	// Shape:
	//	   / \
	//	  /	  \
	//	 /	   \
	//	/____\

	setPoint(0u, sf::Vector2f(10, 0));
	setPoint(1u, sf::Vector2f(20, 30));
	setPoint(2u, sf::Vector2f(0, 30));

	setOrigin(10.f, 15.f);

	setFillColor(sf::Color::Blue);

	setOutlineThickness(1.f);
	setOutlineColor(sf::Color::Cyan);

	refireTime_ = .5f;
}

Player::~Player() {
	
}

void Player::update() {
	move(velocity_);

	ticks_ = (ticks_ + 1) % INT_MAX;

	//Cool weapon
	coolDown_ -= tickToSec(1);
}

void Player::onCollide(Ship* other) {
}